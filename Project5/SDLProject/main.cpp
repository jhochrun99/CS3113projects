#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Map.h"
#include "Entity.h"
#include "Util.h"

#define ENEMY_COUNT 3

GLuint fontTextureID;

Mix_Music* music;
Mix_Chunk* bounce;

Scene* previousScene;
Scene* currentScene;
Scene* sceneList[4];

enum GameMode { START, PLAY, END };

GameState state;
GameMode mode; 

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

void SwitchToScene(Scene* scene) {
    previousScene = currentScene;
    currentScene = scene;
    currentScene->Initialize();

    if (previousScene != NULL) {
        currentScene->PlayerPass(previousScene->state.player);
    }
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Project 5: Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mode = START; //set initial game mode 

    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);

    if (mode == START) {
        music = Mix_LoadMUS("sneaky-adventure-background.mp3");
        Mix_PlayMusic(music, -1);

        bounce = Mix_LoadWAV("jump.wav");
    }
}

//all of the code for processing input
void ProcessInputStart() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LSHIFT: //LSHIFT, RETURN
                mode = PLAY;
                
                SwitchToScene(sceneList[currentScene->state.nextScene]);
                currentScene->state.player->isActive = true;
                
                break;
            }
            break;
        }
    }
}

void ProcessInputPlay() {
    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_SPACE: 
                    if(currentScene->state.player->collidedBottom) { Mix_PlayChannel(-1, bounce, 0); }
                    currentScene->state.player->jump = true;
                    
                    break;
                }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
}

void ProcessInputEnd() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LSHIFT:
                currentScene->state.player->isActive = false;

                mode = START;
                currentScene = sceneList[0];
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
}

void ProcessInput() {
    switch (mode) { //current modes: START, PLAY, END
        case START:
            ProcessInputStart();
            break;
        case PLAY:
            ProcessInputPlay();
            break;
        case END:
            ProcessInputEnd();
            break;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

int endOfScene;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (currentScene->state.player->health == 0) {
        currentScene->state.player->isActive = false;
        mode = END;
        return;
    }

    if (currentScene->state.nextScene == 2) { //level 1
        endOfScene = 15;
    }
    else {
        endOfScene = 18;
    }

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > endOfScene) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-endOfScene, 3.75, 0));
    }
    else if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    if (currentScene->state.map->lastTile == LAVA || currentScene->state.map->lastTile ==  SPIKE) {
        currentScene->state.player->Player::Health();
    }

    accumulator = deltaTime;
}

//all of the code for rendering 
void RenderStart() {
    currentScene->Render(&program);
}

void RenderPlay() {
    currentScene->Render(&program);
}

float endValues[2] = { 0, 0 };
void RenderEnd() {
    currentScene->Render(&program);
    
    endValues[0] = currentScene->state.nextScene;
    endValues[1] = currentScene->state.player->position.x;

    if (currentScene->state.player->isActive) {
        currentScene->Scene::EndMessage(&program, endValues, true);
        currentScene->state.player->canMove = false;
    }
    else {
        currentScene->Scene::EndMessage(&program, endValues, false);
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    switch (mode) { //current modes: START, PLAY, END
        case START:
            RenderStart();
            break;
        case PLAY:
            RenderPlay();
            break;
        case END:
            RenderEnd();
            break;
    }

    SDL_GL_SwapWindow(displayWindow);
}

//shutdown and main
void Shutdown() {
    Mix_FreeChunk(bounce);
    Mix_FreeMusic(music);

    SDL_Quit();
}

Scene* toScene; 
int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        
        if (currentScene->state.map->lastTile == GEM) {
            toScene = sceneList[currentScene->state.nextScene];
            SwitchToScene(toScene);
        }
        else if (currentScene->state.map->lastTile == GOAL && currentScene->state.player->position.x >= 21.5f) {
            mode = END;
        }

        Render();
    }

    Shutdown();
    return 0;
}
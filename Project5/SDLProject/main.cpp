#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

#include "Scene.h"
#include "Level1.h"
#include "Map.h"
#include "Entity.h"
#include "Util.h"

#define ENEMY_COUNT 3

GLuint fontTextureID;

Scene* currentScene;
Level1* level1;

enum GameMode { START, PLAY, END };

GameState state;
GameMode mode; 

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 4: Rise of the AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

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

    glClearColor(0.34f, 0.2f, 0.09f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mode = START; //set initial game mode 

    level1 = new Level1();
    SwitchToScene(level1);
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
            case SDLK_LSHIFT:
                mode = PLAY;

                currentScene->state.player->isActive = true;
                break;
            case SDLK_SPACE:
                break;
            }
            break; // SDL_KEYDOWN
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
                currentScene->state.player->position = glm::vec3(-3.5f, 2.0f, 0);
                currentScene->state.player->velocity = glm::vec3(0);
                currentScene->state.player->movement = glm::vec3(0);
                currentScene->state.player->isActive = false;

                mode = START;
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

//all of the code for updating
int enemiesDead = 0;
void UpdateGameMode() {
    if (!currentScene->state.player->isActive) {
        mode = END;
        return;
    }

    //for (int i = 0; i < ENEMY_COUNT; i++) {
    //    if (!currentScene->state.enemies[i].isActive) {
    //        enemiesDead++;
    //    }
    //}

    if (enemiesDead == ENEMY_COUNT) {
        mode = END;
    }

    enemiesDead = 0;
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void UpdatePlay(float deltaTime) {
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    UpdateGameMode();
    accumulator = deltaTime;
}


void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    switch (mode) { //current modes: START, PLAY, END
        //case START:
        //    UpdateStart(deltaTime);
        //    break;
        case PLAY:
            UpdatePlay(deltaTime);
            break;
        //case END:
        //    UpdateEnd(deltaTime);
        //    break;
    }
}

//all of the code for rendering 
void RenderStart() {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(&program, fontTextureID, "Press left shift to Start!", 0.5f, -0.25f,
        glm::vec3(-2.8f, 1.0f, 0));

    currentScene->Render(&program);
}

void RenderPlay() {
    currentScene->Render(&program);
}

void RenderEnd() {
    currentScene->Render(&program);
    
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    if (state.player->isActive) {
        Util::DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f,
            glm::vec3(-2.0f, 0, 0));
    }
    else {
        Util::DrawText(&program, fontTextureID, "Game Over.", 0.5f, -0.25f,
            glm::vec3(-1.65f, 0, 0));
    }
    Util::DrawText(&program, fontTextureID, "(press left shift to reset)", 0.30f, -0.15f,
        glm::vec3(-1.76f, -0.5f, 0));
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
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
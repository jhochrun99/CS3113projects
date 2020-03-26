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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 10 //determine how many platforms to make
#define WALL_COUNT 12 //determines how tall walls are; should be even value
#define GOAL_SIZE 2

enum GameMode { START, PLAY, END };

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* walls;
    Entity* goal;
};

GameState state;
GameMode mode; 

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
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

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mode = START; //set initial game mode 

    // Initialize Game Objects
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("platformIndustrial_grey.png");
    float locationPlatform = -4.5f;
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(locationPlatform, -3.5f, 0);
        locationPlatform++;
        state.platforms[i].Update(0, NULL, 0); //update platforms once to get them to move to set position
        state.platforms[i].canMove = false; //in position, will never move again
    }

    state.walls = new Entity[WALL_COUNT];
    GLuint wallTextureID = LoadTexture("platformIndustrial_dark.png");
    float locationWall = -2.5f;
    for (int i = 0; i < WALL_COUNT; i+=2) {
        //draws 2 wall blocks each loop, one on left and one on right
        state.walls[i].entityType = PLATFORM;
        state.walls[i].textureID = wallTextureID;
        state.walls[i].position = glm::vec3(-4.5f, locationWall, 0);
        state.walls[i].Update(0, NULL, 0); //update platforms once to get them to move to set position
        state.walls[i].canMove = false; //in position, will never move again

        state.walls[i+1].entityType = PLATFORM;
        state.walls[i+1].textureID = wallTextureID;
        state.walls[i+1].position = glm::vec3(4.5f, locationWall, 0);
        state.walls[i+1].Update(0, NULL, 0); //update platforms once to get them to move to set position
        state.walls[i+1].canMove = false; //in position, will never move again
        locationWall++;
    }

    state.goal = new Entity[GOAL_SIZE];
    GLuint goalTexture1 = LoadTexture("platformIndustrial_101.png");
    GLuint goalTexture2 = LoadTexture("platformIndustrial_103.png");
    float goalPosition = 2.0f;
    for (int i = 0; i < GOAL_SIZE; i++) {
        state.goal[i].entityType = GOAL;
        state.goal[i].position = glm::vec3(goalPosition, -2.5f, 0);
        state.goal[i].Update(0, NULL, 0); //update platforms once to get them to move to set position
        state.goal[i].canMove = false; //in position, will never move again
        goalPosition++;
    }
    state.goal[0].textureID = goalTexture1;
    state.goal[1].textureID = goalTexture2;

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(-3.5f, 4.0f, 0);
    state.player->acceleration = glm::vec3(0, -1.81f, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("george_0.png");
    state.player->height = 0.8f;

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
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
            case SDLK_SPACE:
                mode = PLAY;
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
}

void ProcessInputPlay() {
    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
}

void ProcessInputEnd() {
    //can press space to restart
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
#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void UpdateStart(float deltaTime) {

}

void UpdatePlay(float deltaTime) {
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}

void UpdateEnd(float deltaTime) {

}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    switch (mode) { //current modes: START, PLAY, END
        case START:
            UpdateStart(deltaTime);
            break;
        case PLAY:
            UpdatePlay(deltaTime);
            break;
        case END:
            UpdateEnd(deltaTime);
            break;
    }
}

//all of the code for rendering 
void RenderStart() {
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < WALL_COUNT; i++) {
        state.walls[i].Render(&program);
    }

    for (int i = 0; i < GOAL_SIZE; i++) {
        state.goal[i].Render(&program);
    }
}

void RenderPlay() {
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < WALL_COUNT; i++) {
        state.walls[i].Render(&program);
    }

    for (int i = 0; i < GOAL_SIZE; i++) {
        state.goal[i].Render(&program);
    }

    state.player->Render(&program);
}

void RenderEnd() {

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

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
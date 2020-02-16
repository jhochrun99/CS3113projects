//#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL.h>
//#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shuriken.h"
#include "Character.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, modelMatrix;

struct GameState {
    Character* player;
    Shuriken* leftShuriken;
    Shuriken* rightShuriken;
    Shuriken* centerShuriken;
};

GameState state;

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
    displayWindow = SDL_CreateWindow("Project 1: Simple Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    //shows background color and allows transparency 
    glClearColor(0.6f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize Game Objects (Shuriken)
    state.leftShuriken = new Shuriken(glm::vec3(2, 2, 0));
    state.leftShuriken->speed = 1.0f;
    state.leftShuriken->rotation = -2.0f;
    state.leftShuriken->textureID = LoadTexture("shuriken.png");

    state.rightShuriken = new Shuriken(glm::vec3(2, -2, 0));
    state.rightShuriken->speed = 1.0f;
    state.rightShuriken->rotation = -2.0f;
    state.rightShuriken->textureID = LoadTexture("shuriken.png");

    state.centerShuriken = new Shuriken(glm::vec3(3, 0, 0));
    state.centerShuriken->speed = 1.0f;
    state.centerShuriken->rotation = 5.0f;
    state.centerShuriken->textureID = LoadTexture("shuriken.png");

    // Initialize Character
    state.player = new Character();
    state.player->position = glm::vec3(-4, 0, 0);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("ninjaCharm.png");
}

float lastTicks = 0.0f;

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    state.leftShuriken->Update();
    state.rightShuriken->Update();
    state.centerShuriken->Update();

    state.player->Update(deltaTime);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    state.leftShuriken->Render(&program);
    state.rightShuriken->Render(&program);
    state.centerShuriken->Render(&program);
    state.player->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}


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
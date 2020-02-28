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
#include "Player.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Player* charmander;
    Player* squirtle;
    //if i define another type for the circle, add here
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
    displayWindow = SDL_CreateWindow("Project 2: Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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


    // Initialize Game Objects
    // put circle or pokeball here

    // Initialize Players
    state.charmander = new Player();
    state.charmander->position = glm::vec3(-4.5, 0, 0);
    state.charmander->movement = glm::vec3(0);
    state.charmander->speed = 1.0f;
    state.charmander->textureID = LoadTexture("ninjaCharm.png");

    state.squirtle = new Player();
    state.squirtle->position = glm::vec3(4.5, 0, 0);
    state.squirtle->movement = glm::vec3(0);
    state.squirtle->speed = 1.0f;
    state.squirtle->textureID = LoadTexture("ninjaCharm.png");
}

void ProcessInput() {
    state.charmander->movement = glm::vec3(0);
    state.squirtle->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        //case SDL_KEYDOWN:
        //    switch (event.key.keysym.sym) {
        //    case SDLK_SPACE:
        //        // start ball moving
        //        break;
        //    }
        //    break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W]) {
        state.charmander->movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        state.charmander->movement.y = -1.0f;
    }

    if (keys[SDL_SCANCODE_UP]) {
        state.squirtle->movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        state.squirtle->movement.y = -1.0f;
    }

   /* if (glm::length(state.charmander->movement) > 1.0f) {
        state.charmander->movement = glm::normalize(state.charmander->movement);
    }*/
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    state.charmander->Update(deltaTime);
    state.squirtle->Update(deltaTime);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    state.charmander->Render(&program);
    state.squirtle->Render(&program);

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
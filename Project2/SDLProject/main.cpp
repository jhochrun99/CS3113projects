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
#include "Ball.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Player* charmander;
    Player* squirtle;
    Ball* pokeball;
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
    glClearColor(0.15f, 0.04f, 0.21f, 1.0f); //dark purple color; RGB code / 255
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Ball
    state.pokeball = new Ball();
    state.pokeball->position = glm::vec3(0);
    state.pokeball->movement = glm::vec3(0);
    state.pokeball->speed = 2.5f;
    state.pokeball->textureID = LoadTexture("pokeballTRp.png");

    // Initialize Players ("paddles")
    state.charmander = new Player();
    state.charmander->position = glm::vec3(-4.5, 0, 0);
    state.charmander->movement = glm::vec3(0);
    state.charmander->speed = 2.0f;
    state.charmander->textureID = LoadTexture("ninjaCharmTRp.png");

    state.squirtle = new Player();
    state.squirtle->position = glm::vec3(4.5, 0, 0);
    state.squirtle->movement = glm::vec3(0);
    state.squirtle->speed = 2.0f;
    state.squirtle->textureID = LoadTexture("squirtleTRp.png");
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

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE: //start game with spacebar
                state.pokeball->startGame();
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W]) { //move charmander with w and s keys
        state.charmander->movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        state.charmander->movement.y = -1.0f;
    }

    if (keys[SDL_SCANCODE_UP]) { //move squirtle with up and down arrows
        state.squirtle->movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        state.squirtle->movement.y = -1.0f;
    }

    if (glm::length(state.pokeball->movement) > 1.0f) { //keep ball speed constant
        state.pokeball->movement = glm::normalize(state.pokeball->movement);
    }
}

//function for checking if the ball collides with either Player
Player* activePlayer; //will hold whichever player ball is closer to
float xDist, yDist;
void checkCollision() { //check if ball hits either player
    if (state.pokeball->position.x >= 0) { //on right side
        activePlayer = state.squirtle;
    }
    else { //on left side
        activePlayer = state.charmander;
    }

    //calculating the distance between the ball and player for both x and y axes
    xDist = (fabs(activePlayer->position.x - state.pokeball->position.x))
        - (activePlayer->width + state.pokeball->size)/2;
    yDist = (fabs(activePlayer->position.y - state.pokeball->position.y))
        - (activePlayer->height + state.pokeball->size) / 2;

    if (xDist < 0 && yDist < 0) { //if both distances are < 0, there's a collision
        state.pokeball->movement.x = -state.pokeball->movement.x;
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    state.charmander->Update(deltaTime);
    state.squirtle->Update(deltaTime);
    checkCollision();
    state.pokeball->Update(deltaTime);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    state.charmander->Render(&program);
    state.squirtle->Render(&program);
    state.pokeball->Render(&program);

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
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define FLOOR_COUNT 20 //determine how many floor tiles
#define WALL_COUNT (14*2) //determines how tall walls are; should be even value
#define PLATFORM_COUNT (FLOOR_COUNT*2)+WALL_COUNT
#define ENEMY_COUNT 4
#define FIREBALL_MAX 5

enum GameMode { START, PLAY, END };

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* enemies;
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

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });

    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
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

    // Initialize Game Objects
    float terrainScale = 0.5f;

    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("Tileset.png");
    float locationPlatform = -4.75f;
    float locationWall = -3.0f;

    int* floorIndex = new int[1]{ 1 };
    int* leftWallIndex = new int[1]{ 12 };
    int* rightWallIndex = new int[1]{ 10 };
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].textureCols = 10;
        state.platforms[i].textureRows = 6;

        state.platforms[i].scale = terrainScale;
        state.platforms[i].height *= terrainScale;
        state.platforms[i].width *= terrainScale;

        if (i < FLOOR_COUNT) { //draw floors
            state.platforms[i].animIndices = floorIndex;
            state.platforms[i].position = glm::vec3(locationPlatform, -3.5f, 0);
            locationPlatform += terrainScale;
        }
        else if (i < FLOOR_COUNT + WALL_COUNT/2) { //draw right walls
            state.platforms[i].animIndices = leftWallIndex;
            state.platforms[i].position = glm::vec3(-4.75f, locationWall, 0);
            locationWall += terrainScale;
        }
        else if (i < FLOOR_COUNT + WALL_COUNT) { //draw left walls
            locationWall -= terrainScale;
            state.platforms[i].animIndices = rightWallIndex;
            state.platforms[i].position = glm::vec3(4.8f, locationWall, 0);
        }
        else { //draw ceiling
            locationPlatform -= terrainScale;
            state.platforms[i].animIndices = floorIndex;
            state.platforms[i].position = glm::vec3(locationPlatform, 3.75, 0);
        }

        state.platforms[i].Update(0, NULL, 0); //update platforms once to get them to move to set position
        state.platforms[i].canMove = false; //in position, will never move again
    }

    //Initialize Enemies
    state.enemies = new Entity[ENEMY_COUNT];

    //slime enemy - moves back and forth
    state.enemies[0].enemyType = SLIME;
    state.enemies[0].enemyState = IDLE;
    state.enemies[0].textureID = LoadTexture("slime.png");
    state.enemies[0].textureCols = 8;
    state.enemies[0].textureRows = 3;
    state.enemies[0].height = 0.5f;
    state.enemies[0].width = 0.9f;

    state.enemies[0].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[0].animRight = new int[8]{ 8, 9, 10, 11, 12, 13, 14, 15 };
    state.enemies[0].animUp = new int[8]{ 16, 17, 18, 19, 20, 21, 22, 23 };
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].animFrames = 8;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;

    state.enemies[0].position = glm::vec3(3, -2.8f, 0);

    //bat enemy - follows player
    state.enemies[1].enemyType = BAT;
    state.enemies[1].enemyState = IDLE;
    state.enemies[1].textureID = LoadTexture("batTrp.png");
    state.enemies[1].textureCols = 6;
    state.enemies[1].textureRows = 2;
    state.enemies[1].height = 0.1f;
    state.enemies[1].width = 0.1f;

    state.enemies[1].animLeft = new int[5]{ 0, 1, 2, 3, 4 };
    state.enemies[1].animRight = new int[5]{ 7, 8, 9, 10, 11};
    state.enemies[1].animUp = new int[2]{ 5, 5 };
        //i have no idea why it doesn't work with "5" in animUp only once
    state.enemies[1].animIndices = state.enemies[1].animUp;
    state.enemies[1].animFrames = 2;

    state.enemies[1].position = glm::vec3(-0.5, 3.0f, 0);
    state.enemies[1].senseRadius = 3.0f;

    //fire enemy - shoots fireballs
    GLuint fireTextureID = LoadTexture("explosionTrp.png");
    state.enemies[2].enemyType = FIRE;
    state.enemies[2].enemyState = IDLE;
    state.enemies[2].textureID = fireTextureID;
    state.enemies[2].textureCols = 5;
    state.enemies[2].textureRows = 3;
    state.enemies[2].height = 0.6f;
    state.enemies[2].width = 0.4f;

    state.enemies[2].animUp = new int[4]{ 3, 4, 5, 4};
    state.enemies[2].animLeft = new int[4]{ 5, 6, 7, 8 }; //left for turning off
    state.enemies[2].animIndices = state.enemies[2].animUp;
    state.enemies[2].animFrames = 4;

    state.enemies[2].position = glm::vec3(0, 0, 0);
    state.enemies[2].senseRadius = 3.0f;

    //fireballs for fire enemy
    state.enemies[3].enemyType = FIREBALL;
    state.enemies[3].textureID = fireTextureID;
    state.enemies[3].textureCols = 5;
    state.enemies[3].textureRows = 3;
    state.enemies[3].height = 0.2f;
    state.enemies[3].width = 0.2f;

    state.enemies[3].animUp = new int[1]{ 0 };
    state.enemies[3].animIndices = state.enemies[3].animUp;
    state.enemies[3].animFrames = 1;

    state.enemies[3].position = glm::vec3(0, 0, 0);
    state.enemies[3].movement = glm::vec3(0);
    state.enemies[3].senseRadius = 3.0f;

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 1.0f;
        state.enemies[i].Update(0, NULL, 0);
    }

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(-3.5f, 2.0f, 0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("george_0.png");
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->senseRadius = 0.8f;
    state.player->jumpHeight = 5.0f;

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->textureCols = 4;
    state.player->textureRows = 4;
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
                //have bat and fire start looking for player
                state.enemies[0].enemyState = ATTACKING;
                state.enemies[0].movement = glm::vec3(-1, 0, 0);
                state.enemies[1].senseFor = state.player; 
                state.enemies[2].senseFor = state.player;
                state.player->isActive = true;
                break;
            case SDLK_SPACE:
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

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_SPACE: //player jumps
                    state.player->jump = true;
                    break;
                }
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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LSHIFT: //can press space to clear
                state.player->position = glm::vec3(-3.5f, 2.0f, 0);
                state.player->velocity = glm::vec3(0);
                state.player->movement = glm::vec3(0);
                //state.player->isActive = true;
                for (int i = 0; i < ENEMY_COUNT; i++) {
                    state.enemies[i].isActive = true;
                }
                state.enemies[0].enemyState = IDLE;
                state.enemies[0].position = glm::vec3(3, -2.8f, 0);
                state.enemies[0].animIndices = state.enemies[0].animLeft;
                state.enemies[0].movement = glm::vec3(0);

                state.enemies[1].enemyState = IDLE;
                state.enemies[1].animIndices = state.enemies[1].animUp;
                state.enemies[1].animFrames = 2;
                state.enemies[1].position = glm::vec3(-0.5, 3.0f, 0);
                state.enemies[1].movement = glm::vec3(0);

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
    if (!state.player->isActive) {
        mode = END;
        return;
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (!state.enemies[i].isActive) {
            enemiesDead++;
        }
    }

    if (enemiesDead == ENEMY_COUNT) {
        mode = END;
        return;
    }
    else {
        enemiesDead = 0;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0.0f;
float accumulator = 0.0f;

void UpdateStart(float deltaTime) {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
    }
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

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
    }

    state.player->CheckEnemyCollision(state.enemies, ENEMY_COUNT);

    UpdateGameMode();

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
    GLuint fontTextureID = LoadTexture("font1.png");
    DrawText(&program, fontTextureID, "Press left shift to Start!", 0.5f, -0.25f,
        glm::vec3(-2.8f, 1.0f, 0));

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
}

void RenderPlay() {
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);
}

void RenderEnd() {
    GLuint fontTextureID = LoadTexture("font1.png");

    if (state.player->isActive) {
        DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f,
            glm::vec3(-2.0f, 0, 0));
    }
    else {
        DrawText(&program, fontTextureID, "Game Over.", 0.5f, -0.25f,
            glm::vec3(-1.65f, 0, 0));
    }
    DrawText(&program, fontTextureID, "(press left shift to reset)", 0.30f, -0.15f,
        glm::vec3(-1.76f, -0.5f, 0));

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    for (int i = 0; i < ENEMY_COUNT; i++) {
        //state.enemies[i].position.z = -0.1f;
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);
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
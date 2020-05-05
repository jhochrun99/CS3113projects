#include "Level1.h"
#define LEVEL1_WIDTH 21
#define LEVEL1_HEIGHT 8

#define ENEMY_COUNT 1

unsigned int level1_data[] = {
    3, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 51, 85, 85, 45,
    45, 85, 85, 1, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0,  0, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0, 85, 85, 85, 85, 85, 45,
    45, 91, 91, 91, 91, 85, 85, 85, 85,  91,  0,  0, 85, 85, 85, 85, 85, 85, 85, 85, 45,
     0,  0,  0,  0,  91, 85, 85, 85,  0,  3,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     3,  3,  3,  3,  3,  9,  9,  9,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

void Level1::Initialize() {
    state.nextScene = 2;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 17, 12);
    

    state.player = new Player();
    
    state.player->height= 0.6f;
    state.player->width= 0.6f;

    GLuint slimeTextureID = Util::LoadTexture("slime.png");
    state.enemy1 = new Enemy();
    state.enemy1->DefineSlime(slimeTextureID);
    state.enemy1->position = glm::vec3(14.0f, -5.0f, 0);

    state.enemy2 = new Enemy();
    state.enemy2->DefineSlime(slimeTextureID);
    state.enemy2->position = glm::vec3(6.0f, -6.0f, 0);
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemy1, state.enemy2, ENEMY_COUNT);

    state.enemy1->Update(deltaTime, state.map, NULL, NULL, 0);
    state.enemy2->Update(deltaTime, state.map, NULL, NULL, 0);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_DOWN] ) {
        state.player->CheckDown(state.map);
    }
    if (keys[SDL_SCANCODE_RIGHT] ) {
        state.player->CheckRight(state.map);
    }
    if (keys[SDL_SCANCODE_LEFT] ) {
        state.player->CheckLeft(state.map);
    }

}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);

    state.enemy1->Render(program);
    state.enemy2->Render(program);

    state.player->Render(program);
    
}

void Level1::PlayerPass(Player* prevPlayer) {
    //no previous player for level 1
}

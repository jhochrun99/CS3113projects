#include "Level1.h"
#define LEVEL1_WIDTH 21
#define LEVEL1_HEIGHT 8

#define ENEMY_COUNT 1

unsigned int level1_data[] = {
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 51, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0,  0, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0,  0, 85, 85, 85, 85, 85, 85, 85, 85, 45,
     0,  0,  0,  0,  0, 85, 85, 85,  0,  3,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     3,  3,  3,  3,  3,  0,  0,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

void Level1::Initialize() {
    state.nextScene = 2;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 14, 7);

    state.player = new Player();

    state.enemies = new Enemy[ENEMY_COUNT];
    GLuint slimeTextureID = Util::LoadTexture("slime.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].enemy = SLIME;
        state.enemies[i].textureID = slimeTextureID;

        state.enemies[i].textureCols = 8;
        state.enemies[i].textureRows = 3;
        state.enemies[i].height = 0.5f;
        state.enemies[i].width = 0.9f;

        state.enemies[i].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
        state.enemies[i].animRight = new int[8]{ 8, 9, 10, 11, 12, 13, 14, 15 };
        state.enemies[i].animUp = new int[8]{ 16, 17, 18, 19, 20, 21, 22, 23 };
        state.enemies[i].animIndices = state.enemies[i].animLeft;
        state.enemies[i].animFrames = 8;
        state.enemies[i].movement = glm::vec3(-1, 0, 0);
    }
    //state.enemies[0].position = glm::vec3(6.0f, -6.0f, 0);
    state.enemies[0].position = glm::vec3(14.0f, -5.0f, 0);
    //state.enemies[2].position = glm::vec3(18.0f, -5.0f, 0);
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemies, ENEMY_COUNT);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.map, NULL, 0);
    }
}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);
}

void Level1::PlayerPass(Player* prevPlayer) {
    //no previous player for level 1
}
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
    state.nextScene = 1;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 14, 7);
    // Move over all of the player and enemy code from initialization.

    state.player = new Player();

    state.enemies = new Enemy[ENEMY_COUNT];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].enemy = SLIME;
    state.enemies[0].textureID = Util::LoadTexture("slime.png");
    state.enemies[0].textureCols = 8;
    state.enemies[0].textureRows = 3;
    state.enemies[0].height = 0.5f;
    state.enemies[0].width = 0.9f;

    state.enemies[0].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[0].animRight = new int[8]{ 8, 9, 10, 11, 12, 13, 14, 15 };
    state.enemies[0].animUp = new int[8]{ 16, 17, 18, 19, 20, 21, 22, 23 };
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].animFrames = 8;

    state.enemies[0].position = glm::vec3(6.0f, -6.0f, 0);
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemies, 0);
    state.enemies->Update(deltaTime, state.map, NULL, 0);

    if (state.player->position.x >= 12) {
        //state.nextScene = 1;
    }
}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.enemies->Render(program);
    state.player->Render(program);
}
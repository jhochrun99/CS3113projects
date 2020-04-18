#include "Level2.h"
#define LEVEL2_WIDTH 24
#define LEVEL2_HEIGHT 8

unsigned int level2_data[] = {
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
    59, 15, 15, 15, 15, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 15, 15, 85, 85, 85, 59,
    59, 85, 85, 85, 85, 85, 15, 15, 15, 85, 15, 85, 85, 85, 85, 85, 15, 85, 85, 85, 85, 85, 52, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 15, 15, 85, 85, 85, 85, 85, 85, 15, 15, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19
};

void Level2::Initialize() {
    state.nextScene = 2;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 14, 7);

    state.player = new Player();
    state.player->position = glm::vec3(2.0f, 1.0f, 0);
    state.player->health = playerHealth;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemies, 0);
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
}
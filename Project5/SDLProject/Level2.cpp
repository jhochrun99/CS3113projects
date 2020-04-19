#include "Level2.h"
#define LEVEL2_WIDTH 24
#define LEVEL2_HEIGHT 8

#define ENEMY_COUNT 1

unsigned int level2_data[] = {
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
    59, 15, 15, 15, 15, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 15, 15, 85, 85, 85, 59,
    59, 85, 85, 85, 85, 85, 15, 15, 15, 85, 15, 85, 85, 15, 85, 85, 15, 85, 85, 85, 85, 85, 52, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 15, 85, 85, 85, 85, 85, 15, 15, 15, 59,
    59, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 59,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19
};

void Level2::Initialize() {
    state.nextScene = 3;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 14, 7);

    state.player = new Player();

    state.enemies = new Enemy[ENEMY_COUNT];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].enemy = BAT;
    state.enemies[0].textureID = Util::LoadTexture("bat.png");

    state.enemies[0].textureCols = 6;
    state.enemies[0].textureRows = 2;
    state.enemies[0].height = 0.2f;
    state.enemies[0].width = 0.7f;

    state.enemies[0].animLeft = new int[5]{ 0, 1, 2, 3, 4 };
    state.enemies[0].animRight = new int[5]{ 7, 8, 9, 10, 11 };
    state.enemies[0].animUp = new int[2]{ 5, 5 };
    state.enemies[0].animIndices = state.enemies[0].animUp;
    state.enemies[0].animFrames = 2;

    state.enemies[0].position = glm::vec3(8.0f, -0.35f, 0);
    state.enemies[0].senseRadius = 4.0f;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemies, ENEMY_COUNT);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.map, NULL, 0);
    }

    if (state.player->position.y <= -5.2f) { //player has been in lava too long
        state.player->Player::Health();
    }
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);
}

void Level2::PlayerPass(Player* prevPlayer) {
    state.player = prevPlayer;
    state.player->position = glm::vec3(2.0f, 1.0f, 0);

    state.enemies[0].senseFor = state.player;
}
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

    GLuint batTextureId = Util::LoadTexture("bat.png");
    state.enemy1 = new Enemy();
    state.enemy1->DefineBat(batTextureId);
    state.enemy1->position = glm::vec3(15.0f, -0.35f, 0);

    GLuint slimeTextureID = Util::LoadTexture("slime.png");
    state.enemy2 = new Enemy();
    state.enemy2->DefineSlime(slimeTextureID);
    state.enemy2->position = glm::vec3(10.0f, -2.0f, 0);
    state.enemy2->movement = glm::vec3(0);
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemy1, state.enemy2, ENEMY_COUNT);

    state.enemy1->Update(deltaTime, state.map, NULL, NULL, 0);
    state.enemy2->Update(deltaTime, state.map, NULL, NULL, 0);

    if (state.player->position.y <= -5.2f) { //player has been in lava too long
        state.player->Player::Health();
    }
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);

    state.enemy1->Render(program);
    state.enemy2->Render(program);

    state.player->Render(program);
}

void Level2::PlayerPass(Player* prevPlayer) {
    state.player = prevPlayer;
    state.player->position = glm::vec3(2.0f, 1.0f, 0);

    state.enemy1->senseFor = state.player;
}
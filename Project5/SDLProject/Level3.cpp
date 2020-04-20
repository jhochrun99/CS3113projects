#include "Level3.h"
#define LEVEL3_WIDTH 24
#define LEVEL3_HEIGHT 8

unsigned int level3_data[] = {
    60, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    60, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  4,
    60, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 18,
    60, 85, 85, 85, 85, 85, 85, 17, 85, 85, 85, 85, 85, 85, 85, 17, 85, 85, 85, 85, 85, 85, 17, 18,
    60, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 17, 85, 85, 85, 85, 17, 85, 17, 85, 85, 18,
    60, 85, 85, 85, 85, 85, 85, 85, 85, 17, 85, 17, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 49, 18,
    60, 85, 85, 85, 85, 85, 85, 17, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 17, 18,
    60, 70, 70, 70, 70, 17, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 18
};

void Level3::Initialize() {
    state.nextScene = 0;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 14, 7);

    state.player = new Player();

    GLuint batTextureId = Util::LoadTexture("bat.png");
    state.enemy1 = new Enemy();
    state.enemy1->DefineBat(batTextureId);
    state.enemy1->position = glm::vec3(15.0f, -0.35f, 0);
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, state.enemy1, NULL, 1);

    state.enemy1->Update(deltaTime, state.map, NULL, NULL, 0);

    if (state.player->position.y <= -6.5f) {
        state.player->Player::Health();
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemy1->Render(program);
}

void Level3::PlayerPass(Player* prevPlayer) {
    state.player = prevPlayer;
    state.player->position = glm::vec3(5.0f, 1.0f, 0);

    state.enemy1->senseFor = state.player;
}
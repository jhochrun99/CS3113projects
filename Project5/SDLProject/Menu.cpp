#include "Menu.h"
#define MENU_WIDTH 21
#define MENU_HEIGHT 8

#define ENEMY_COUNT 3

unsigned int menu_data[] = {
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 51, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0,  0, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0, 85, 85, 85, 85, 85, 45,
    45, 85, 85, 85, 85, 85, 85, 85, 85,  0,  0,  0, 85, 85, 85, 85, 85, 85, 85, 85, 45,
     0,  0,  0,  0,  0, 85, 85, 85,  0,  3,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     3,  3,  3,  3,  3,  0,  0,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
};

void Menu::Initialize() {
    state.nextScene = 1;

    state.player = new Player();
    state.player->canMove = false;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 14, 7);

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
    state.enemies[0].position = glm::vec3(6.0f, -6.0f, 0);
    state.enemies[1].position = glm::vec3(14.0f, -5.0f, 0);
    state.enemies[2].position = glm::vec3(18.0f, -5.0f, 0);
}

void Menu::Update(float deltaTime) {
    //state.player->Update(deltaTime, state.map, NULL, 0);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.map, NULL, 0);
    }
}

void Menu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Gem Dash", 1.0f, -0.5f,
        glm::vec3(3.5f, -2.5f, 0));

    //state.player->Render(program);
    state.map->Render(program);

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}
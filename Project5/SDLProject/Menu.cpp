#include "Menu.h"
#define MENU_WIDTH 21
#define MENU_HEIGHT 8

#define ENEMY_COUNT 2

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

    GLuint slimeTextureID = Util::LoadTexture("slime.png");
    state.enemy1 = new Enemy();
    state.enemy1->DefineSlime(slimeTextureID);
    state.enemy1->position = glm::vec3(14.0f, -5.0f, 0);

    state.enemy2 = new Enemy();
    state.enemy2->DefineSlime(slimeTextureID);
    state.enemy2->position = glm::vec3(6.0f, -6.0f, 0);
}

void Menu::Update(float deltaTime) {
    state.enemy1->Update(deltaTime, state.map, NULL, NULL, 0);
    state.enemy2->Update(deltaTime, state.map, NULL, NULL, 0);
}

void Menu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Gem Dash", 1.0f, -0.5f,
        glm::vec3(3.5f, -2.5f, 0));

    state.map->Render(program);

    state.enemy1->Render(program);
    state.enemy2->Render(program);
}

void Menu::PlayerPass(Player* prevPlayer) {
    //no prevPlayer for menu
}
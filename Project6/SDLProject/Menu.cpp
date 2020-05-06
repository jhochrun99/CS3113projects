#include "Menu.h"
#define MENU_WIDTH 11
#define MENU_HEIGHT 8

#define ENEMY_COUNT 2

unsigned int menu_data[] = {
    28, 85, 85, 85, 85, 85, 85, 85, 85, 85, 28,
    28, 45, 45, 85, 85, 85, 85, 85, 85, 85, 28,
    28, 85, 85, 85, 85, 85, 85, 85, 85, 85, 28,
    28, 85, 85, 85, 85, 85, 85, 85, 85, 85, 28,
    28, 85, 85, 85, 85, 85, 85, 85, 85, 85, 28,
    28, 85, 85, 85, 85, 85, 85, 85, 85,  0, 28,
    28,  0,  0,  0,  0, 85, 85, 85,  0,  3, 28,
    28,  3,  3,  3,  3,  0,  0,  0,  3,  3, 28
};

void Menu::Initialize() {
    state.nextScene = 1;

    state.player = new Player();
    state.player->canMove = false;

    GLuint mapTextureID = Util::LoadTexture("mapPack_tilesheet.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 17, 12);

    //GLuint slimeTextureID = Util::LoadTexture("slime.png");
    //state.enemy1 = new Enemy();
    //state.enemy1->DefineSlime(slimeTextureID);
    //state.enemy1->position = glm::vec3(14.0f, -5.0f, 0);
}

void Menu::Update(float deltaTime) {
//update enemies
}

void Menu::Render(ShaderProgram* program) {
    GLuint fontTextureID2 = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID2, "Press enter to see instructions", 0.65f, -0.4f,
                        glm::vec3(1.3f, -4.5f, 0));

    state.map->Render(program);
}

void Menu::PlayerPass(Player* prevPlayer) {
    //no prevPlayer for menu
}

#include "Menu.h"
#define MENU_WIDTH 21
#define MENU_HEIGHT 8

#define ENEMY_COUNT 2

unsigned int menu_data[] = {
   18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6, 85, 85, 6, 85, 85, 6, 85, 6, 6,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6,
    6, 85, 85, 85, 85, 85, 85, 85, 85, 85, 6, 85, 85, 85, 85, 85, 85, 85, 85, 6, 6,
   18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18
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
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawTextTitle(program, fontTextureID, "c", 1.0f, -0.1f,
        glm::vec3(1.8f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "h", 1.0f, -0.1f,
                   glm::vec3(2.5f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "f", 1.0f, -0.1f,
                   glm::vec3(3.0f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "c", 1.0f, -0.1f,
                   glm::vec3(4.3f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "d", 1.0f, -0.1f,
                   glm::vec3(5.0f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "d", 1.0f, -0.1f,
                   glm::vec3(5.8f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "o", 0.85f, -0.1f,
                   glm::vec3(6.6f, -2.68f, 0));
    Util::DrawTextTitle(program, fontTextureID, "d", 1.0f, -0.1f,
                   glm::vec3(7.3f, -2.5f, 0));
    Util::DrawTextTitle(program, fontTextureID, "q", 0.85f, -0.1f,
                   glm::vec3(8.1f, -2.68f, 0));
    
    GLuint fontTextureID2 = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID2, "Press enter to see instructions", 0.65f, -0.4f,
                        glm::vec3(1.3f, -4.5f, 0));

    state.map->Render(program);

    state.enemy1->Render(program);
    state.enemy2->Render(program);
}

void Menu::PlayerPass(Player* prevPlayer) {
    //no prevPlayer for menu
}

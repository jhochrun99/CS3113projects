#include "Level1.h"
#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 82

#define ENEMY_COUNT 6
#include <vector>
#include <tuple>

//for one screen: 8 tall, 11 across
//blocks key: [dirt, 91], [sand, 18], [rock, 28], [ice, 202],  [nonrendered, 85]
//            ["blank", 16], [tower, 110], [castle, 111]
//            [spikes, 84], [lava, 83, 100]
unsigned int level1_data[] = {
    28,  85,  85,  85,  85,  85,  85,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  85,  85,  85,  91,  91,  91,  85,  91,  91,  28,
    28,  84,  84,  84,  91,  91,  91,  84,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  84,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28, // 8
    28,  91,  91,  91,  85,  85,  91,  85,  85,  85,  28,
    28,  91,  91,  91,  91,  91,  91,  84,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  84,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  84,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  28,  91,  91,  91,  28,
    28,  91,  28,  91,  91,  91,  91,  28,  91,  91,  28,
    28,  28,  91,  91,  85,  91,  91,  91,  91,  91,  28, // 16
    28,  91,  91,  84,  91,  84,  91,  91,  91,  91,  28,
    28,  91,  28,  28,  84,  91,  91,  91,  91,  91,  28,
    28,  28,  91,  28,  91,  91,  91,  91,  91,  91,  28,
    28, 110,  91,  91,  91,  91,  91,  28,  28,  91,  28, // checkpoint
    28,  91,  91,  91,  91,  91,  91,  91,  91,  28,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  85,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  28,  28,  91,  91,  91,  28, // 24
    28,  91,  91,  28,  28,  91,  28,  84,  28,  85,  28,
    28,  91,  91,  91,  28,  91,  91,  91,  91,  91,  28,
    28,  84,  84,  91,  91,  91,  28,  91,  91,  91,  28,
    28,  91,  91,  84,  28,  91,  28,  91,  84,  84,  28,
    28,  91,  91,  91,  91,  91,  91,  28,  91,  91,  28,
    28,  85,  85,  28,  28,  91,  91,  91,  91,  91,  28,
    28,  28,  91,  91,  91,  84,  91,  91,  84,  91,  28,
    28,  91,  28,  28,  91,  28,  91,  84,  91,  91,  28, // 32
    28,  91,  91,  91,  91,  28,  28,  91,  91,  85,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  85,  28,
    28,  91,  91,  85,  85,  85,  91,  28,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  28,  91,  28,
    28,  85,  84,  91,  91,  91,  84,  84,  91,  91,  28,
    28,  91,  28,  85,  85,  85,  28,  28,  85,  85,  28,
    28,  91,  85,  85,  91,  91,  91,  91,  91,  28,  28,
    28, 110,  85,  91,  28,  85,  85,  85,  85,  28,  28, // 40 checkpoint, SLIME IN THIS ROW
    28,  28,  91,  91,  91,  91,  85,  85,  85,  91,  28,
    28,  91,  28,  91,  28,  91,  28,  28,  28,  91,  28,
    28,  91,  91,  85,  85,  91,  91,  91,  91,  91,  28,
    28,  84,  91,  91,  91,  85,  85,  28,  91,  91,  28,
    28,  91,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  85,  85,  85,  85,  91,  91,  91,  85,  28, // SLIME IN THIS ROW
    28,  91,  91,  91,  91,  91,  91,  85,  85,  85,  28, // SLIME IN THIS ROW
    28,  91,  84,  85,  85,  85,  91,  91,  91,  91,  28, // 48
    28,  91,  85,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91,  28,  91,  91,  91,  91,  28,
    28,  85,  85,  85,  85,  85,  85,  91,  84,  91,  28, // SLIME IN THIS ROW
    28,  84,  28,  91,  91,  91,  91,  85,  85,  91,  28,
    28,  91,  91,  91,  91,  85,  85,  91,  91,  91,  28,
    28,  91,  85,  91,  85,  91,  91,  91,  91,  91,  28,
    28,  85,  91,  91,  91,  91,  91,  91,  91,  84,  28,
    28, 202, 202, 202, 202, 202, 202, 202, 202, 202,  28, // 56
    28,  91,  91,  91,  91,  85,  91,  91,  28,  28,  28,
    28,  85, 202, 202,  91,  91,  91,  85,  85,  91,  28,
    28,  84,  84,  85,  91, 202,  91,  91,  91,  91,  28,
    28, 110,  91,  91,  91,  91,  91, 202,  91,  91,  28, // checkpoint
    28,  91,  91,  91,  85,  85,  85,  91, 202,  91,  28, 
    28,  91,  84,  85,  91,  91,  91,  85,  85,  91,  28,
    28,  91,  91,  91,  91,  83,  83,  83,  91,  91,  28,
    28,  91,  91,  91,  91, 100, 100, 100,  91,  91,  28, // 64
    28,  91,  91,  91,  91,  28,  28,  28,  85,  85,  28,
    28, 202,  91,  28,  91,  91,  91,  85,  91,  91,  28,
    28,  91,  91,  85,  85,  91,  91,  91,  91,  91,  28,
    28,  91,  91,  91,  91, 202,  84,  91,  28,  91,  28,
    28,  85,  85,  85,  85,  85,  91,  85,  91,  28,  28,
    28,  91, 202,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  84,  91,  91,  91,  91,  91,  91,  91,  91,  28,
    28,  85,  85,  85,  85,  91,  91,  85,  91,  91,  28, // 72 // SLIME IN THIS ROW
    28,  91,  91,  91,  91,  85,  85,  91,  91,  91,  28,
    28, 202, 202, 202, 202, 202, 202, 202, 202,  85,  28,
    28,  83,  83,  83,  83,  83,  83,  83, 202,  85,  28,
    28,  85,  85,  28,  28,  85,  85,  85,  85,  85,  28,
    28, 202, 202,  91,  91,  91,  91, 202,  91,  91,  28,
    28,  28,  91,  91, 202,  91, 202,  91,  91,  91,  28,
    28, 202, 202,  85,  85,  84,  91,  91, 202, 202,  28, // SLIME IN THIS ROW
    28,  83,  83,  85,  85,  85,  91,  91,  83,  83,  28, // 80
    28, 100, 100,  28,  28, 111,  91,  91, 100, 100,  28,
    28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28
};

std::vector<Sand*> sand_blocks;

void Level1::Initialize() {
    state.nextScene = 0;
    
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 17, 12);
    state.player = new Player();
    state.player->height= 0.6f;
    state.player->width= 0.2f;
    
    GLuint slimeTextureID = Util::LoadTexture("slime.png");
 
    state.enemies = new Enemy[ENEMY_COUNT];
    state.enemies[0].DefineSlime(slimeTextureID);
    state.enemies[0].position = glm::vec3(5.0f, -40.0f, 0);
    state.enemies[0].acceleration= glm::vec3(0, -12.81f, 0);
    state.enemies[1].DefineSlime(slimeTextureID);
    state.enemies[1].position = glm::vec3(2.0f, -46.0f, 0);
    state.enemies[1].acceleration= glm::vec3(0, -12.81f, 0);
    state.enemies[2].DefineSlime(slimeTextureID);
    state.enemies[2].position = glm::vec3(7.0f, -47.0f, 0);
    state.enemies[2].acceleration= glm::vec3(0, -9.81f, 0);
    state.enemies[3].DefineSlime(slimeTextureID);
    state.enemies[3].position = glm::vec3(1.0f, -51.0f, 0);
    state.enemies[3].acceleration= glm::vec3(0, -9.81f, 0);
    state.enemies[4].DefineSlime(slimeTextureID);
    state.enemies[4].position = glm::vec3(1.0f, -72.0f, 0);
    state.enemies[4].acceleration= glm::vec3(0, -9.81f, 0);
    state.enemies[5].DefineSlime(slimeTextureID);
    state.enemies[5].position = glm::vec3(3.0f, -79.0f, 0);
    state.enemies[5].acceleration= glm::vec3(0, -9.81f, 0);
    
    std::tuple<int, int> sand_coordinates[] = {
        {9,-33},{3,-34},{4,-34},{4,-34},
        {1, -36},{8, -37},{9,-37},{2, -38},
        {3, -38}, {5, -39}, {6, -40}, //{2, -39},
        {7, -40}, {8, -40}, {3, -42}, {4, -42},
        {9,-45}, {3, -47}, {4, -47}, {5, -47},
        {2, -48}, {1, -50}, {2, -50}, {7, -51},
        {8, -51}, {5, -52}, {6, -52}, {2, -53},
        {4, -53}, {1, -54}, {5, -56}, {1, -57},
        {7, -57}, {8, -57}, {3, -58}, {4,-60},
        {5, -60}, {6, -60},{3, -61}, {7,-65},
        {3, -66}, {4, -66}, {1,-68}, {2, -68},
        {3, -68}, {4, -68}, {5, -68}, {7, -68},
        {1, -71}, {2, -71}, {3, -71}, {4, -71},
        {7,-71}, {9,-73}, {9, -74}, {1, -75},
        {2, -75}, {5, -75}, {6, -75}, {7, -75},
        {8, -75}, {9, -75}, {4,-78}, {3, -79},
        {4, -79}
    };

    for (int i=0; i < 65; i++){
        Sand* new_sand = new Sand();
        new_sand->position.x = std::get<0>(sand_coordinates[i]);
        new_sand->position.y = std::get<1>(sand_coordinates[i]);
        new_sand->acceleration = glm::vec3(0, -2.81f, 0);
        new_sand->entityType = SANDD;
        new_sand->height = 1.0f;
        sand_blocks.push_back(new_sand);
    }
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.map, NULL, NULL, 0);
    
    for(int i = 0; i < sand_blocks.size(); i++) {
        sand_blocks[i]->Update(deltaTime, state.map, state.player, state.player, 1);
    }

    for(int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.map, NULL, NULL, 0);
        state.player->Update(deltaTime, state.map, &state.enemies[i], NULL, 1);
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_DOWN]) {
        state.player->CheckDown(state.map);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->CheckRight(state.map);
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->CheckLeft(state.map);
    }
}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);

    for(int i = 0; i < sand_blocks.size(); i++){
        sand_blocks[i]->Render(program);
        sand_blocks[i]->DrawSpriteFromTextureAtlas(program, sand_blocks[i]->textureID, 17);
    }

    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }

    GLuint fontTexture = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTexture, "Lives: "+ std::to_string(state.player->health), 0.7f, -0.45f, glm::vec3(7, state.player->position.y+1, 0));

    state.player->Render(program);
}

void Level1::PlayerPass(Player* prevPlayer) {
    //no previous player for level 1
}

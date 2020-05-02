#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Enemy.h"

struct GameState {
    Map* map;
    Player* player;
    Enemy* enemy1;
    Enemy* enemy2;

    int nextScene;
};

class Scene {
public:
    GameState state;

    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram* program) = 0;

    virtual void PlayerPass(Player* prevPlayer) = 0;
    
    void EndMessage(ShaderProgram* program, float endPosition[], bool win);
};
#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

enum EnemyType { SLIME, BAT, BLADE };
enum EnemyState { IDLE, ATTACKING };

class Enemy : public Entity {
public:
    EnemyType enemy;
    EnemyState enemyState;
    Entity* senseFor;
    int senseRadius;

    Enemy();

    void Health() override;
    void Action() override;

    void CheckSense(Entity* senseFor);

    void Slime();
    void Bat();
};
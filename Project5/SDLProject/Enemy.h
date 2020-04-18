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

class Enemy : public Entity {
public:
    EnemyType enemy;

    Enemy();

    void Action() override;
    void Slime();
};
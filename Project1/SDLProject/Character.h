#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Character {
public:
    glm::vec3 position;
    float speed;
    bool moveForward = true; 

    GLuint textureID;
    glm::mat4 modelMatrix;

    Character();

    void Update(float deltaTime);
    void Render(ShaderProgram* program);
};
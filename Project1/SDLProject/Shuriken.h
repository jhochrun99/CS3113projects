#pragma once

//not sure if needed, but runs without
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Shuriken {
public:
    glm::vec3 position;
    float speed;
    float rotation;

    GLuint textureID;
    glm::mat4 modelMatrix;

    Shuriken(glm::vec3 pos);

    void Update();
    void Render(ShaderProgram* program);
};
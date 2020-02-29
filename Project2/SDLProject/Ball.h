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

class Ball {
public:
    glm::vec3 position;
    glm::vec3 previousPosition;
    glm::vec3 movement;
    float speed;
    float size; //square, doesn't need height/width
    float yBoundary;
    float xBoundary;

    GLuint textureID;

    glm::mat4 modelMatrix;

    Ball();

    void Update(float deltaTime);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void startGame();
};
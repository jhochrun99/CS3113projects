#include "Ball.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Ball::Ball() {
    //position = glm::vec3(0);
    speed = 0;
    size = 0.42f; //both based off visible part of sprite
    yBoundary = 3.75f;
    xBoundary = 5.0f;

    /*modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);*/
}

void Ball::Update(float deltaTime) {
    position += movement * speed * deltaTime;

    bool movingUp = position.y > 0;
    bool movingRight = position.x > 0;

    //won't let Ball move past top of window
    //if (movingUp && (position.y + size / 2) < yBoundary) {
    //    modelMatrix = glm::mat4(1.0f);
    //    modelMatrix = glm::translate(modelMatrix, position);
    //} //won't let Ball move past bottom of window
    //else if (!movingUp && (position.y - size / 2) > -yBoundary) {
    //    modelMatrix = glm::mat4(1.0f);
    //    modelMatrix = glm::translate(modelMatrix, position);
    //}
    
    ////won't let Ball move past right of window
    if (movingRight && (position.x + size / 2) < xBoundary) { 
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    } //won't let Ball move past left of window
    else if (!movingRight && (position.x - size / 2) > -xBoundary) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
}

void Ball::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index) {
    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Ball::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Ball::startGame() {
    position = glm::vec3(0);

    srand(time(NULL));
    //get a random x and y value
    for (int i = 0; i < 2; i++) { 
        if (i == 0) { //x value
            movement.x = 1 + (rand() % 10);
        }
        else { //y value
            movement.y = 1 + (rand() % 10);
        }
    }
}
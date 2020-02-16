#include "Character.h"

Character::Character() {
    position = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

void Character::Update(float deltaTime) {
    //moves character forward to a certain point
    //then back to a certain point
    //then back and forth between those two points
    if (position.x < 2  && moveForward) {
        position.x += speed * deltaTime;
    } else if (position.x >= 2 || !moveForward) {
        if (position.x < 1) {
            moveForward = true;
        } else {
            moveForward = false;
        }
        position.x -= speed * deltaTime;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Character::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);
    //apply texture
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
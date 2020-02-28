#include "Player.h"

Player::Player() {
    position = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Player::Update(float deltaTime) {
    position += movement * speed * deltaTime;
    
    bool movingUp = position.y > 0;
    float top = 3.75;
    float bottom = -3.75;
    if (movingUp && position.y < top) { //won't let Player move past top of window
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    } else if (!movingUp && position.y > bottom) { //won't let Player move past bottom of window
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
}

void Player::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Player::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
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
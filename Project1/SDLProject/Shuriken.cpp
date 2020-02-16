#include "Shuriken.h"

Shuriken::Shuriken(glm::vec3 pos) { 
    //take in a position vector, so each Shuriken's position can be different
    position = pos;
    speed = 0;
    rotation = 0;

    //move the shuriken to their initial position instead of the origin
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Shuriken::Update(/*float deltaTime*/) {
    rotation = rotation * speed;
    //continuously rotate the shuriken
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Shuriken::Render(ShaderProgram* program) {
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
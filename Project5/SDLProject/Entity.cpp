#include "Entity.h"
#include "math.h"
#include <stdlib.h>
#include <time.h>
#include "Util.h"
#include "Enemy.h"

Entity::Entity() {
    srand(time(NULL));

    position = glm::vec3(0);
    velocity = glm::vec3(0);
    acceleration = glm::vec3(0);
    movement = glm::vec3(0);
    speed = 1.0f;

    jump = false;
    maxVal = 0;

    width = 1.0f;
    height = 1.0f;
    scale = 1.0f;

    isActive = true;
    canMove = true;
    canUse = true;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedWith = NULL;

    modelMatrix = glm::mat4(1.0f);

    animRight = NULL;
    animLeft = NULL;
    animUp = NULL;
    animDown = NULL;

    animIndices = NULL;
    animFrames = 0;
    animIndex = 0;
    animTime = 0;
    textureCols = 0;
    textureRows = 0;
}

bool Entity::CheckCollision(Entity* other) {
    if (other == NULL) { return false; }
    if (!isActive || !other->isActive) { return false; }

    float xDist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float yDist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xDist < 0 && yDist < 0) {
        return true;
    }

    return false;
}

void Entity::CheckCollisionY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];

        if (CheckCollision(object)) {
            float yOverlap = fabs(fabs(position.y - object->position.y)
                - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) {
                position.y -= yOverlap;
                velocity.y = 0;
                collidedTop = true;
                collidedWith = object;
            }
            else if (velocity.y < 0) {
                position.y += yOverlap;
                velocity.y = 0;
                collidedBottom = true;
                collidedWith = object;
            }
        }
    }
}

void Entity::CheckCollisionX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];

        if (CheckCollision(object)) {
            float xOverlap = fabs(fabs(position.x - object->position.x)
                - (width / 2.0f) - (object->width / 2.0f));

            if (velocity.x > 0) {
                position.x -= xOverlap;
                velocity.x = 0;
                collidedRight = true;
                collidedWith = object;
            }
            else { //if (velocity.x < 0) {
                position.x += xOverlap;
                velocity.x = 0;
                collidedLeft = true;
                collidedWith = object;
            }
        }
    }
}

void Entity::CheckCollisionsY(Map* map) {
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }

    if (map->lastTile == LAVA || map->lastTile == SPIKE) {
        //life--;
        map->lastTile = SAFE;
    }
}

void Entity::CheckCollisionsX(Map* map) {
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::Update(float deltaTime, Map* map, Entity* objects, int objectCount) {
    if (!isActive || !canMove) { return; } //don't do anything if not active

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || entityType != PLAYER) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;
        }
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionY(objects, objectCount); // Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionX(objects, objectCount); // Fix if needed

    if (jump) {
        jump = false;
        if (collidedBottom) {
            velocity.y += maxVal;
        }
    }

    if (entityType == ENEMY) { Action(); }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    if (!isActive) { return; } //don't do anything if not active

    float u = (float)(index % textureCols) / (float)textureCols;
    float v = (float)(index / textureCols) / (float)textureRows;

    float width = 1.0f / (float)textureCols;
    float height = 1.0f / (float)textureRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    //default size is 0.5
    float sizeNeg = -0.5 * scale;
    float sizePos = 0.5 * scale;

    float vertices[] = { sizeNeg, sizeNeg, sizePos, sizeNeg, sizePos, sizePos,
        sizeNeg, sizeNeg, sizePos, sizePos, sizeNeg, sizePos };
    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    //default size is 0.5
    float sizeNeg = -0.5 * scale;
    float sizePos = 0.5 * scale;

    float vertices[] = { sizeNeg, sizeNeg, sizePos, sizeNeg, sizePos, sizePos, 
        sizeNeg, sizeNeg, sizePos, sizePos, sizeNeg, sizePos };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 
        1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



Player::Player() {
    health = 3;

    position = glm::vec3(4.0f, 1.0f, 0);
    acceleration = glm::vec3(0, -9.81f, 0);
    speed = 2.0f;
    textureID = Util::LoadTexture("george_0.png");
    height = 0.8f;
    width = 0.75f;
    maxVal = 5.5f;

    animRight = new int[4]{ 3, 7, 11, 15 };
    animLeft = new int[4]{ 1, 5, 9, 13 };
    animUp = new int[4]{ 2, 6, 10, 14 };
    animDown = new int[4]{ 0, 4, 8, 12 };

    animIndices = animRight;
    animFrames = 4;
    animIndex = 0;
    animTime = 0;
    textureCols = 4;
    textureRows = 4;
}

void Player::Action() {}

void Player::Health() {

}
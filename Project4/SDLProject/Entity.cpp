#include "Entity.h"
#include "math.h"
#include <stdlib.h>
#include <time.h>

Entity::Entity() {
    position = glm::vec3(0);
    velocity = glm::vec3(0);
    acceleration = glm::vec3(0);
    movement = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
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
            //float yDist = fabs(position.y - object->position.y);
            float yOverlap = fabs(fabs(position.y - object->position.y) - (height / 2.0f) - (object->height / 2.0f));
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
            //float xDist = fabs(position.x - object->position.x);
            float xOverlap = fabs(fabs(position.x - object->position.x) - (width / 2.0f) - (object->width / 2.0f));

            if (velocity.x > 0) {
                position.x -= xOverlap;
                velocity.x = 0;
                collidedRight = true;
                collidedWith = object;
            }
            else if (velocity.x < 0) {
                position.x += xOverlap;
                velocity.x = 0;
                collidedLeft = true;
                collidedWith = object;
            }
        }
    }
}

void Entity::CheckSense(Entity* senseFor) {
    if (senseFor == NULL) { return; }

    //distance between player's center and enemy's center
    float distance = sqrt(pow(senseFor->position.x - position.x, 2) + pow(senseFor->position.y - position.y, 2));
    float sumSenseRadii = senseRadius + senseFor->senseRadius;

    //within sensing range
    if (distance < sumSenseRadii) {
        enemyState = ATTACKING;
    }
    else {
        enemyState = IDLE;
    }
}

glm::vec3 fireballMovement = glm::vec3(0);
int fireballMaxCount = 5;
glm::vec3 Entity::ShootFire() {
    srand(time(NULL));
    //get a random x and y value
    for (int i = 0; i < 2; i++) {
        if (i == 0) { //x value
            fireballMovement.x = (1 + (rand() % 10));
        }
        else { //y value
            fireballMovement.y = (1 + (rand() % 10));
        }
    }

    return fireballMovement;
}

void Entity::Slime() {
    switch (enemyState) {
        case IDLE:

            break; //currently do nothing
        case ATTACKING:
            /*movement = glm::vec3(-1, 0, 0);*/
            if (collidedLeft) {
                movement.x = -movement.x;
                animIndices = animRight;
            }
            else if (collidedRight) {
                movement.x = -movement.x;
                animIndices = animLeft;
            }
            break;
    }
    
}

void Entity::Bat() {
    switch (enemyState) {
        case IDLE:
            CheckSense(senseFor);
            break;
        case ATTACKING:
            if (!senseFor->isActive) { break; }

            movement = glm::vec3(senseFor->position.x - position.x, 
                senseFor->position.y - position.y, 0);

            if (movement.x > 0) {
                animIndices = animRight;
                animFrames = 5;
            } 
            else {
                animIndices = animLeft;
                animFrames = 5;
            }
            break;
    }
}

void Entity::Fire() {
    switch (enemyState) {
        case IDLE: //doesn't shoot
            CheckSense(senseFor);
            
            break;
        case ATTACKING: //shoots randomly when player is in range
            ShootFire();
            CheckSense(senseFor);
            break;
        case DEAD:
            //changes appearance
            break;
    }
}

void Entity::Enemy() {
    if (!isActive) { return; } //do nothing if not active
    switch (enemyType) {
        case SLIME:
            Slime();
            break;
        case BAT:
            Bat();
            break;
        case FIRE:
            Fire();
            break;
    }
}

void Entity::CheckEnemyCollision(Entity* enemies, int enemyCount) {
    if (!isActive) { return; } //don't do anything if not active
    if (!canMove) { return; } //can't move, doesn't need to be updated

    CheckCollisionY(enemies, enemyCount);
    CheckCollisionX(enemies, enemyCount);
    
    //player dies
    if ((collidedTop || collidedLeft || collidedRight) && collidedWith->entityType == ENEMY) {
        movement = glm::vec3(0);
        isActive = false;
    } //enemy dies, unless enemy is fire
    else if (collidedBottom && collidedWith->entityType == ENEMY) {
        if (collidedWith->enemyType != FIRE) {
            collidedWith->isActive = false;
        }
        else {
            movement = glm::vec3(0);
            isActive = false;
        }
    }
}

void Entity::Update(float deltaTime, Entity* platforms, int platformCount)
{
    if (!isActive) { return; } //don't do anything if not active
    if (!canMove) { return; } //can't move, doesn't need to be updated

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || entityType == FIRE) {
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
    
    position.y += velocity.y * deltaTime;
    CheckCollisionY(platforms, platformCount);
    position.x += velocity.x * deltaTime;
    CheckCollisionX(platforms, platformCount);

    if (entityType == ENEMY) { Enemy(); }

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
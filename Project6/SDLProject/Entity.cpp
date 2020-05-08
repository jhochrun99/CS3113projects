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

    health = 3;

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
            
            if(entityType==SANDD){
                if(position.y > object->position.y+0.5){
                    object->Health();
                    if (object->health == 1) {
                        object->isActive = false;
                    }
                }
                else{
                    return;
                }
                
                
                return;
            }
            //if player falls on top of the sand  player.position>sand.position
            //if sand falls on top of player sand.position> player.position
            float yOverlap = fabs(fabs(position.y - object->position.y)
                - (height / 2.0f) - (object->height / 2.0f));
            
            if(object->entityType==ENEMY){
                if(position.y > object->position.y+0.2){
                    
                    object->isActive = false;
                    
                }
                else{
                    std::cout << "this happens!!"<<std::endl;
                    Health();
                    return;
                }
                
                
                return;
            }
            if (velocity.y > 0) {
                //std::cout <<"is this happening1"<<std::endl;
               
                position.y -= yOverlap;
                velocity.y = 0;
                collidedTop = true;
                collidedWith = object;

                Health();
            }
            else if (velocity.y < 0) {
                //std::cout <<"is this happening2"<<std::endl;
                if(entityType==SANDD){
                    return;

//                    std::cout << "player lives is "<< object->health<<std::endl;
                    
                }
      
                position.y += yOverlap;
                velocity.y = 0;
                collidedBottom = true;
                collidedWith = object;

                if (object->health == 1) {
                    object->isActive = false;
                }
                else {
                    object->health--;
                    
                    
                }
            }
        }
    }
}

void Entity::CheckCollisionX(Entity* objects, int objectCount) {
    
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        //std::cout<< "i is "<< i<< std::endl;
        if (object->entityType==ENEMY){
            //std::cout<< "Enemy #"<< i<< std::endl;
        }
        if (CheckCollision(object)) {
            float xOverlap = fabs(fabs(position.x - object->position.x)
                - (width / 2.0f) - (object->width / 2.0f));

            if (velocity.x > 0) {
                std::cout << "this happens"<<std::endl;
                position.x -= xOverlap;
                velocity.x = 0;
                collidedRight = true;
                collidedWith = object;

                Health();
                
            }
            else {
                std::cout << "this happens too"<<std::endl;
                position.x += xOverlap;
                velocity.x = 0;
                collidedLeft = true;
                collidedWith = object;

                Health();
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
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    if (entityType==SANDD){
        if (map->IsSolidSand(top, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        else if (map->IsSolidSand(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        else if (map->IsSolidSand(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        
        if (map->IsSolidSand(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSand(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSand(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        
        if (map->IsSolidSand(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSand(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSand(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        return;
        
    }
    if (entityType==ENEMY){
        
        if (map->IsSolidSlime(top, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        else if (map->IsSolidSlime(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        else if (map->IsSolidSlime(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
            position.y -= penetration_y;
            velocity.y = 0;
            collidedTop = true;
        }
        
        if (map->IsSolidSlime(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
            
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSlime(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSlime(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        
        if (map->IsSolidSlime(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSlime(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolidSlime(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        return;
        
    }
    
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
        glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
        if(map->currentTile==GLASS && int(ticks)%2==0){
            map->destroy_tile(bottom);
        }
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
void Entity::CheckDown(Map *map){

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    
    if(map->currentTile==DIRT){
        map->destroy_tile(bottom);
    }

    
    
}
void Entity::CheckRight(Map *map){
    glm::vec3 right = glm::vec3(position.x + (width), position.y, position.z);
    if(map->rightTile==DIRT){
        map->destroy_tile(right);
    }

    
    
}
void Entity::CheckLeft(Map *map){

    glm::vec3 left = glm::vec3(position.x - (width), position.y, position.z);
    if(map->leftTile==DIRT){
        map->destroy_tile(left);
    }
}


void Entity::CheckCollisionsX(Map* map) {
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    
    if(entityType==ENEMY){
        if (map->IsSolidSand(left, &penetration_x, &penetration_y) && velocity.x < 0) {
            position.x += penetration_x;
            velocity.x = 0;
            collidedLeft = true;
        }
        
        if (map->IsSolidSand(right, &penetration_x, &penetration_y) && velocity.x > 0) {
            position.x -= penetration_x;
            velocity.x = 0;
            collidedRight = true;
        }
    }
    
  
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


void Entity::Update(float deltaTime, Map* map, Entity* objects, Entity* obj2, int objectCount) {
    if (!isActive || !canMove) { return; } //don't do anything if not active
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

   
    if (animIndices != NULL && objects==NULL) {
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
    
    if((entityType==PLAYER && objects==NULL) || entityType!=PLAYER){
        velocity.x = movement.x * speed;
        velocity += acceleration * deltaTime;
    }
    
    
    if(entityType==SANDD){
        position.y += velocity.y * deltaTime;
        CheckCollisionY(objects,objectCount);
        position.x += velocity.x * deltaTime;
        CheckCollisionsY(map);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        return;
        
    }
    
    if((entityType==PLAYER && objects==NULL) || entityType!=PLAYER){
        position.y += velocity.y * deltaTime; // Move on Y
    }

    

    
    CheckCollisionsY(map);
    
    CheckCollisionY(objects, objectCount); // Fix if needed
    //CheckCollisionY(obj2, objectCount);

    if((entityType==PLAYER && objects==NULL) || entityType!=PLAYER){
        position.x += velocity.x * deltaTime;
    }
     // Move on X
    CheckCollisionsX(map);
    CheckCollisionX(objects, objectCount); // Fix if needed
    //CheckCollisionX(obj2, objectCount);

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
    isActive = true;

    position = glm::vec3(2.0f, 0, 0);
    acceleration = glm::vec3(0, -5.8f, 0);
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
    health -= 1;

    if (health == 0) {
        isActive = false;
    }
    else {
        movement = glm::vec3(0);
        position = glm::vec3(4.0f, 1.0f, 0);
    }
}

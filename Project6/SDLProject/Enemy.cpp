#include "Enemy.h"

Enemy::Enemy() {
    senseFor = NULL;
    enemyState = IDLE;
    entityType = ENEMY;
}

void Enemy::DefineSlime(GLuint slimeTextureID) {
    enemy = SLIME;

    textureID = slimeTextureID;
    textureCols = 8;
    textureRows = 3;
    height = 0.8f;
    width = 0.9f;
    animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    animRight = new int[8]{ 8, 9, 10, 11, 12, 13, 14, 15 };
    animUp = new int[8]{ 16, 17, 18, 19, 20, 21, 22, 23 };
    animIndices = animLeft;
    animFrames = 8;
    movement = glm::vec3(-1, 0, 0);
}

void Enemy::DefineBat(GLuint batTextureID) {
    enemy = BAT;

    textureID = batTextureID;
    textureCols = 6;
    textureRows = 2;
    height = 0.2f;
    width = 0.7f;
    animLeft = new int[5]{ 0, 1, 2, 3, 4 };
    animRight = new int[5]{ 7, 8, 9, 10, 11 };
    animUp = new int[2]{ 5, 5 };
    animIndices = animUp;
    animFrames = 2;

    senseRadius = 4.0f;
}

void Enemy::Health() { }

void Enemy::Action() {
    if (!isActive) { return; }

    switch (enemy) {
    case SLIME:
        Slime();
        break;
    case BAT:
        Bat();
        break;
    case BLADE:
        //Blade();
        break;
    }
}

void Enemy::CheckSense(Entity* senseFor) {
    if (senseFor == NULL) { 
        enemyState = IDLE;
        return; 
    }

    //distance between player's center and enemy's center
    float distance = sqrt(pow(senseFor->position.x - position.x, 2) + pow(senseFor->position.y - position.y, 2));
    float sumSenseRadii = senseRadius + (senseFor->height / 2);

    //within sensing range
    if (distance < sumSenseRadii) {
        enemyState = ATTACKING;
    }
    else {
        enemyState = IDLE;
    }
}

void Enemy::Slime() {
    if (collidedLeft) {
        movement.x = -movement.x;
        animIndices = animRight;
    }
    else if (collidedRight) {
        movement.x = -movement.x;
        animIndices = animLeft;
    }
}

void Enemy::Bat() {
    switch (enemyState) {
    case IDLE:
        CheckSense(senseFor);
        break;
    case ATTACKING:
        if (!senseFor->isActive) { break; }

        movement = glm::vec3(senseFor->position.x - position.x,
            senseFor->position.y - position.y, 0);

        velocity.y = movement.y * speed;

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

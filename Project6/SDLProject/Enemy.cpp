#include "Enemy.h"

Enemy::Enemy() {
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

void Enemy::Health() { }

void Enemy::Action() {
    if (!isActive) { return; }

    switch (enemy) {
    case SLIME:
        Slime();
        break;
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


#include "Enemy.h"

Enemy::Enemy() {
    senseFor = NULL;
    enemyState = IDLE;
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
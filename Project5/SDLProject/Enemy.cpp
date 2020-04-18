#include "Enemy.h"

Enemy::Enemy() {

}

void Enemy::Action() {
    if (!isActive) { return; }

    switch (enemy) {
    case SLIME:
        Slime();
        break;
    case BAT:
        //Bat();
        break;
    case BLADE:
        //Blade();
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
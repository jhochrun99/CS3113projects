#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, BUTTON };
enum EnemyType { BAT, SLIME, FIRE, FIREBALL };
enum EnemyState { IDLE, ATTACKING, DEAD };

class Entity {
public:
    EntityType entityType;
    EnemyType enemyType;
    EnemyState enemyState;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 movement;
    float speed;

    bool jump;
    float jumpHeight;

    float width;
    float height;
    float scale;
    float senseRadius;
    Entity* senseFor;

    bool isActive;
    bool canMove;
    bool canUse;

    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    Entity* collidedWith;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight;
    int* animLeft;
    int* animUp;
    int* animDown;

    int* animIndices;
    int animFrames;
    int animIndex;
    float animTime;
    int textureCols;
    int textureRows;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionY(Entity* objects, int objectCount, Entity* button);
    void CheckCollisionX(Entity* objects, int objectCount);
    void CheckSense(Entity* object);

    void CheckEnemyCollision(Entity* platform, int platformCount);
    void Update(float deltaTime, Entity* platform, int platformCount, Entity* button);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void Enemy();
    void Slime();
    void Bat();
    void Fire();
    glm::vec3 ShootFire();
};
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

enum EntityType { PLAYER, PLATFORM, ENEMY };
enum EnemyType { BAT, SLIME, FIRE };
enum EnemyState { IDLE, WALKING, ATTACKING, DEAD };

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

    float width = 1.0f;
    float height = 1.0f;
    float scale = 1.0f;
    float senseRadius = 0.0f;
    Entity* senseFor = NULL;

    bool isActive = true;
    bool canMove = true;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    Entity* collidedWith;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int textureCols = 0;
    int textureRows = 0;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionY(Entity* objects, int objectCount);
    void CheckCollisionX(Entity* objects, int objectCount);
    void CheckSense(Entity* object);

    void CheckEnemyCollision(Entity* platform, int platformCount);
    void Update(float deltaTime, Entity* platform, int platformCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void Enemy();
    void Slime();
    void Bat();
    void Fire();
};
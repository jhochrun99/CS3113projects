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
#include "Map.h"

enum EntityType { PLAYER, ENEMY };

class Entity {
public:
    EntityType entityType;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 movement;
    float speed;

    int health;

    bool jump;
    float maxVal; //used for jump height

    float width;
    float height;
    float scale;

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
    void CheckCollisionY(Entity* objects, int objectCount);
    void CheckCollisionX(Entity* objects, int objectCount);
    void CheckCollisionsY(Map* map);
    void CheckCollisionsX(Map* map);

    virtual void Action() = 0;
    virtual void Health() = 0;

    void Update(float deltaTime, Map* map, Entity* objects, int objectCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};

class Player : public Entity {
public:
    int health; 

    Player();

    void Action() override;
    void Health() override;
};
#include "Map.h"
Map::Map(int width, int height, unsigned int* levelData, GLuint textureID, float tile_size, int
    tile_count_x, int tile_count_y) {
    this->width = width;
    this->height = height;
    this->levelData = levelData;
    this->textureID = textureID;
    this->tile_size = tile_size;
    this->tile_count_x = tile_count_x;
    this->tile_count_y = tile_count_y;

    lastTile = SAFE;

    this->Build();
}

void Map::Build() {
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
  
            int tile = levelData[y * width + x];
            if (tile == 85) continue;

            float u = (float)(tile % tile_count_x) / (float)tile_count_x;
            float v = (float)(tile / tile_count_x) / (float)tile_count_y;

            float tileWidth = 1.0f / (float)tile_count_x;
            float tileHeight = 1.0f / (float)tile_count_y;

            float xoffset = -(tile_size / 2); // From center of tile
            float yoffset = (tile_size / 2); // From center of tile

            vertices.insert(vertices.end(), {
                xoffset + (tile_size * x), yoffset + -tile_size * y,
                xoffset + (tile_size * x), yoffset + (-tile_size * y) - tile_size,
                xoffset + (tile_size * x) + tile_size, yoffset + (-tile_size * y) - tile_size,

                xoffset + (tile_size * x), yoffset + -tile_size * y,
                xoffset + (tile_size * x) + tile_size, yoffset + (-tile_size * y) - tile_size,
                xoffset + (tile_size * x) + tile_size, yoffset + -tile_size * y
            });

            texCoords.insert(texCoords.end(), {
                u, v,
                u, v + (tileHeight),
                u + tileWidth, v + (tileHeight),

                u, v,
                u + tileWidth, v + (tileHeight),
                u + tileWidth, v
            });
        }
    }

    left_bound = 0 - (tile_size / 2);
    right_bound = (tile_size * width) - (tile_size / 2);

    top_bound = 0 + (tile_size / 2);
    bottom_bound = -(tile_size * height) + (tile_size / 2);
};

void Map::Render(ShaderProgram* program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size() / 2);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Map::IsSolid(glm::vec3 position, float* penetration_x, float* penetration_y) {
    *penetration_x = 0;
    *penetration_y = 0;

    if (position.x < left_bound || position.x > right_bound) return false;
    if (position.y > top_bound || position.y < bottom_bound) return false;

    int tile_x = floor((position.x + (tile_size / 2)) / tile_size);
    int tile_y = -(ceil(position.y - (tile_size / 2))) / tile_size; // Our array counts up as Y goes down.
    glm::vec3 position_left = glm::vec3(position.x -(0.3f), position.y + 0.6f, position.z);
    glm::vec3 position_right = glm::vec3(position.x +(0.3f), position.y + 0.6f, position.z);
    
    int tile_x_left = floor((position_left.x + (tile_size / 2)) / tile_size);
    int tile_y_left = -(ceil(position_left.y - (tile_size / 2))) / tile_size;
    
    int tile_x_right = floor((position_right.x + (tile_size / 2)) / tile_size);
    int tile_y_right = -(ceil(position_right.y - (tile_size / 2))) / tile_size;
    
    if (tile_x < 0 || tile_x >= width) return false;
    if (tile_y < 0 || tile_y >= height) return false;

    int tile = levelData[tile_y * width + tile_x];
    int tileLeft = levelData[(tile_y_left * width + tile_x_left)];
    int tileRight = levelData[(tile_y_right * width + tile_x_right)];
    //tiles that can't be hit: 85 (empty), 39, 67 (heart)
    
    if (tile == 85) return false;
    
    if (tile == 15) return false;
    
    if (tile == 18) { currentTile = SAND; }
    else if (tile == 28) { currentTile = METAL; }
    else if (tile == 91) { currentTile = DIRT; }
    else if (tile == 202) { currentTile = GLASS; }
    else { currentTile = OTHER; }

    if (tileLeft == 18) { leftTile = SAND; }
    else if (tileLeft == 28) { leftTile = METAL; }
    else if (tileLeft == 91) { leftTile = DIRT; }
    else if (tileLeft == 86) { leftTile= GLASS; }
    else { leftTile = OTHER; }
    
    if (tileRight == 18) { rightTile = SAND; }
    else if (tileRight == 28) { rightTile = METAL; }
    else if (tileRight == 91) { rightTile = DIRT; }
    else if (tileRight == 86) { rightTile= GLASS; }
    else { rightTile = OTHER; }
    
    if (tile == 85) return false;

    else if (tile == 110) {
        lastTile = CHECKPOINT;
        return false;
    }
    else if (tile == 111) {
        lastTile = GOAL;
        return false;
    }
    
    float tile_center_x = (tile_x * tile_size);
    float tile_center_y = -(tile_y * tile_size);

    if (tile == 84) { 
        lastTile = SPIKE;
        return false;
    }
    else if (tile == 83 || tile == 100) {
        lastTile = LAVA;
    }
    else {
        *penetration_y = (tile_size / 2) - fabs(position.y - tile_center_y);
        *penetration_x = (tile_size / 2) - fabs(position.x - tile_center_x);
    }

    return true;
}

bool Map::IsSolidSand(glm::vec3 position, float* penetration_x, float* penetration_y) {
    *penetration_x = 0;
    *penetration_y = 0;
    
    if (position.x < left_bound || position.x > right_bound) return false;
    if (position.y > top_bound || position.y < bottom_bound) return false;
    
    int tile_x = floor((position.x + (tile_size / 2)) / tile_size);
    int tile_y = -(ceil(position.y - (tile_size / 2))) / tile_size; // Our array counts up as Y goes down.
    
    if (tile_x < 0 || tile_x >= width) return false;
    if (tile_y < 0 || tile_y >= height) return false;
    
    int tile = levelData[tile_y * width + tile_x];
    if (tile == 0) return false;
    if (tile == 85) return false;
    if (tile == 15) return false;
    
    float tile_center_x = (tile_x * tile_size);
    float tile_center_y = -(tile_y * tile_size);
    
    *penetration_x = (tile_size / 2) - fabs(position.x - tile_center_x);
    *penetration_y = (tile_size / 2) - fabs(position.y - tile_center_y);
    return true;
}
bool Map::IsSolidSlime(glm::vec3 position, float* penetration_x, float* penetration_y) {
    *penetration_x = 0;
    *penetration_y = 0;
    
    if (position.x < left_bound || position.x > right_bound) return false;
    if (position.y > top_bound || position.y < bottom_bound) return false;
    
    int tile_x = floor((position.x + (tile_size / 2)) / tile_size);
    int tile_y = -(ceil(position.y - (tile_size / 2))) / tile_size; // Our array counts up as Y goes down.
    
    if (tile_x < 0 || tile_x >= width) return false;
    if (tile_y < 0 || tile_y >= height) return false;
    
    int tile = levelData[tile_y * width + tile_x];
    
    if (tile == 0) return false;
    if (tile == 85) return false;
    if (tile == 15) return false;
    
    float tile_center_x = (tile_x * tile_size);
    float tile_center_y = -(tile_y * tile_size);
    
    *penetration_x = (tile_size / 2) - fabs(position.x - tile_center_x);
    *penetration_y = (tile_size / 2) - fabs(position.y - tile_center_y);
    return true;
}

void Map::destroy_tile(glm::vec3 position) {
    int tile_x = floor((position.x + (tile_size / 2)) / tile_size);
    int tile_y = -(ceil(position.y - (tile_size / 2))) / tile_size; // Our array counts up as Y goes down.
    
    //so basically here i change that tile to nothing
    levelData[tile_y * width + tile_x]= 15;
    this->Build();
}

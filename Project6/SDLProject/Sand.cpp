//
//  Sand.cpp
//  FinalProject
//
//  Created by Angie Beck on 5/5/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Sand.hpp"

Sand::Sand(){
    isActive= true;
    
    position = glm::vec3(1.0f, -5.0f, 0);
    acceleration = glm::vec3(0, -9.81f, 0);

    textureID = Util::LoadTexture("tilesheet.png");
    textureCols = 14;
    textureRows = 12;

    height = 1.0f;
    width = 0.9f;
}

void Sand::Health() { }
void Sand::Action() { }

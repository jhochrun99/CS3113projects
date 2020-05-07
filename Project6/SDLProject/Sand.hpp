//
//  Sand.hpp
//  FinalProject
//
//  Created by Angie Beck on 5/5/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Util.h"

class Sand : public Entity {
public:
    Sand();

    void Health() override;
    void Action() override;
};

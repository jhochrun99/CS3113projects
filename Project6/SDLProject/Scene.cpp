#include "Scene.h"

float xPosition;

void Scene::EndMessage(ShaderProgram* program, float endPosition[], bool win) {
	GLuint fontTextureID = Util::LoadTexture("font1.png");
	if (win) {
		Util::DrawText(program, fontTextureID, "You Win!", 1.0f, -0.5f,
			glm::vec3(3.0f, state.player->position.y+2, 0));
	}
	else {

        
        Util::DrawText(program, fontTextureID, "You lose :(", 1.0f, -0.5f,
			glm::vec3(3.0f, state.player->position.y-2, 0));
	}
}

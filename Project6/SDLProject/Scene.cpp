#include "Scene.h"

float xPosition;

void Scene::EndMessage(ShaderProgram* program, float endPosition[], bool win) {
	GLuint fontTextureID = Util::LoadTexture("font1.png");
	if (win) {
		Util::DrawText(program, fontTextureID, "You Win!", 1.0f, -0.5f,
			glm::vec3(16.5f, -3.0f, 0));
	}
	else {
		if (endPosition[0] == 2) {
			if (endPosition[1] >= 15) {
				xPosition = 13;
			}
			else {
				xPosition = endPosition[1] - 2;
			}
		}
		else {
			if (endPosition[1] >= 18) {
				xPosition = 16;
			}
			else {
				xPosition = endPosition[1] - 2;
			}
		}

		Util::DrawText(program, fontTextureID, "You lose...", 1.0f, -0.5f,
			glm::vec3(xPosition, -3.0f, 0));
	}
}
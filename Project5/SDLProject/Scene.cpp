#include "Scene.h"

void Scene::EndMessage(ShaderProgram* program, bool win) {
	GLuint fontTextureID = Util::LoadTexture("font1.png");
	if (win) {
		Util::DrawText(program, fontTextureID, "You Win!", 1.0f, -0.5f,
			glm::vec3(16.5f, -3.0f, 0));
	}
	else {
		Util::DrawText(program, fontTextureID, "You lose...", 1.0f, -0.5f,
			glm::vec3(3.5f, -3.0f, 0));
	}
}
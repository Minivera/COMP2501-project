#pragma once

#include <glm/glm.hpp>

#include "GameState.h"

class LevelState : public GameState {
private:
	GLuint backgroundTexture = 0;
	GLuint midgroundTexture = 0;
public:
	LevelState(const GLuint* textures);
	
	void controls(glm::vec2 mousePos, double deltaTime);

	void render(Shader& shader);
};
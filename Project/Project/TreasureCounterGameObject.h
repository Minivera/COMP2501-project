#pragma once

#include "GameObject.h"

class TreasureCounterGameObject : public GameObject
{
private:
	// The amount of treasure to display.
	int amount;
public:
	TreasureCounterGameObject(int amount, glm::vec3& entityPos, glm::vec3 entityScale, GLuint treasureTexture, GLint entityNumElements);

	void render(Shader& spriteShader);

	// Setters
	void setAmount(int newAmount) { amount = newAmount; }
};


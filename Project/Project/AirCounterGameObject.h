#pragma once

#include "GameObject.h"

class AirCounterGameObject : public GameObject
{
private:
	// The amount of air to display, in miliseconds.
	float amount;
public:
	AirCounterGameObject(float amount, glm::vec3& entityPos, glm::vec3 entityScale, GLuint treasureTexture, GLint entityNumElements);

	void render(Shader& spriteShader);

	// Setters
	void setAmount(float newAmount) { amount = newAmount; }
};


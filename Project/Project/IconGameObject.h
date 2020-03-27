#pragma once

#include "GameObject.h"

// Concrete implementation of the game object that only display its texture with no interactibility
class IconGameObject : public GameObject
{
public:
	IconGameObject(glm::vec3 entityPosition, glm::vec3 entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
};


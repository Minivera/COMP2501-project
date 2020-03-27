#pragma once
#include "GuiGameObject.h"
class BoxGameObject : public GuiGameObject
{
	// Set if that box has been completed, if it was, we show it in green.
	bool completed = false;
public:
	BoxGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLint entityNumElements);

	// Set the box as completed and changes the textures to show it green.
	void complete();

	// Variables that give the texture IDs for the textures of the various buttons
	static GLuint boxTextureID;
	static GLuint hoveredBoxTextureID;
	static GLuint completedBoxTextureID;

	// Static method to load the button's textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname, int skipX, int skipY, int rowHeight, int colLength), GLuint* textures, int offset);
};


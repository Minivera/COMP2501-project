#pragma once

#include "GuiGameObject.h"

enum class ButtonType { StartButton, QuitButton, ResumeButton, AbandonButton };

class ButtonGameObject: public GuiGameObject
{
public:
	ButtonGameObject(ButtonType type, glm::vec3& entityPosition, glm::vec3& entityScale, GLint entityNumElements);

	// Variables that give the texture IDs for the textures of the various buttons
	static GLuint startButtonTextureID;
	static GLuint startButtonActiveTextureID;
	static GLuint quitButtonTextureID;
	static GLuint quitButtonActiveTextureID;
	static GLuint resumeButtonTextureID;
	static GLuint resumeButtonActiveTextureID;
	static GLuint abandonButtonTextureID;
	static GLuint abandonButtonActiveTextureID;

	// Static method to load the button's textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname, int skipX, int skipY, int rowHeight, int colLength), GLuint* textures, int offset);
};


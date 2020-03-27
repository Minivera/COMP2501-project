#pragma once

#include <map>
#include <string>

#include "GameObject.h"

class TextRenderer
{
private:
	// The map of texture IDs mapped to their chars
	static map<char, GLuint> textureMap;

public:
	// Static method that's used to render a set of text on the screen.
	static void render(const string& word, const glm::mat4& worldMatrix, Shader& spriteShader, GLint numElements);

	// Static method to load all the text textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset);
};


#pragma once

#include "GameObject.h"

#include "TextRenderer.h"

class TextGameObject : public GameObject
{
private:
	// Words we know have a texture already constructed. If the word we are trying to construct is not here, we'll use the text renderer.
	static map<string, GLuint> knownWords;

	// Word this entity is expected to render on screen.
	string word = "";

public:
	TextGameObject(const string& word, glm::vec3 entityPosition, glm::vec3 entityScale, GLint entityNumElements);

	// Overriden render method that will defer to the game object or the text renderer.
	void render(Shader& spriteShader);

	// Setters
	inline void setWord(const string& newWord) { word = newWord; }

	static GLuint textureFor(string word) { return knownWords.at(word); }

	// Static method to load the text textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset);
};


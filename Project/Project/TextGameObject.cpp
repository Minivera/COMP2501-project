#include "TextGameObject.h"

map<string, GLuint> TextGameObject::knownWords;

TextGameObject::TextGameObject(const string& word, glm::vec3 entityPosition, glm::vec3 entityScale, GLint entityNumElements) :
	GameObject(entityPosition, entityScale, 0, 0, entityNumElements) {
	this->word = word;
}

void TextGameObject::render(Shader& spriteShader) {
	// Use the game object to render know words that we already have a texture for.
	if (knownWords.count(word) > 0) {
		texture = knownWords.at(word);
		GameObject::render(spriteShader);
	}
	else {
		// Otherwise, use the text renderer and hope it display properly.
		// Start by moving to the position
		glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

		// Added a scale matrix to scale the sprite so it can be rescaled
		transformationMatrix = glm::scale(transformationMatrix, scale * glm::vec3(0.6f, 0.6f, 1.0f));

		TextRenderer::render(word, transformationMatrix, spriteShader, numElements);
	}
}

int TextGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\gui\\air.png");
	knownWords.insert(make_pair("air", textures[offset + 0]));

	setFuncPtr(textures[offset + 1], "Assets\\gui\\unlock.png");
	knownWords.insert(make_pair("unlock", textures[offset + 1]));

	setFuncPtr(textures[offset + 2], "Assets\\gui\\level.png");
	knownWords.insert(make_pair("level", textures[offset + 2]));
	return offset + 3;
}
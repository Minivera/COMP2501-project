#include "TextRenderer.h"

map<char, GLuint> TextRenderer::textureMap;

int TextRenderer::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
    string letters = "1234567890+-:";

    for (int i = 0; i < letters.length(); i++) {
        string path = "Assets\\gui\\text\\";
        if (letters.at(i) == ':') {
            path += "colon";
        }
        else {
            path += letters.at(i);
        }
        path += ".png";

        setFuncPtr(textures[offset + i], path.c_str());
        textureMap.insert(make_pair(letters.at(i), textures[offset + i]));
    }

    return offset + letters.length();
}

void TextRenderer::render(const string& word, const glm::mat4& worldMatrix, Shader& spriteShader, GLint numElements) {
    // Text is always centered
    glm::vec3 position = glm::vec3(-(word.length() / 4.0f), 0.0f, 0.0f);

    // render each letter individually
    for (int i = 0; i < word.length(); i++) {
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, textureMap.at(word[i]));

		// Move the letter by the position in the world matrix
		glm::mat4 transformationMatrix = glm::translate(worldMatrix, position);

		// Set the transformation matrix in the shader
        spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
        spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
        spriteShader.setUniform1i("count", 0);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
        position.x += 1.0f;
    }
}
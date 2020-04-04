#include "PlayerInterfaceGameObject.h"

#include "TextGameObject.h"
#include "IconGameObject.h"

PlayerInterfaceGameObject::PlayerInterfaceGameObject(shared_ptr<PlayerGameObject> player, glm::vec3 entityPos, glm::vec3& entityScale, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPos, entityScale, 0, entityTexture, entityNumElements) {
	this->player = player;

	loadInterface();
}

void PlayerInterfaceGameObject::loadInterface() {
	glm::vec3 boxScale = scale * glm::vec3(0.6f, 0.6f, 0.8f);
	glm::vec3 markerScale = scale * glm::vec3(0.2f, 0.2f, 1.0f);
	glm::vec3 counterScale = scale * glm::vec3(0.6f, 0.6f, 1.0f);

	// Load the player information
	treasureCounter = make_shared<TreasureCounterGameObject>(
		player->getCurrentTreasure(),
		glm::vec3(),
		glm::vec3(0.8f, 0.8f, 0.8f),
		TreasureGameObject::treasureTextureID,
		6
	);
	subEntities.push_back(treasureCounter);

	airCounter = make_shared<AirCounterGameObject>(
		player->getCurrentAir(),
		glm::vec3(),
		glm::vec3(0.8f, 0.8f, 0.8f),
		TextGameObject::textureFor("air"),
		6
	);
	subEntities.push_back(airCounter);
}

void PlayerInterfaceGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Update things based on the player object
	treasureCounter->setAmount(player->getCurrentTreasure());

	airCounter->setAmount(player->getCurrentAir());

	GameObject::update(entities, deltaTime);
}

void PlayerInterfaceGameObject::render(Shader& spriteShader) {
	float xIncrement = 0.9f;

	glm::vec3 boxPos = position + glm::vec3(-1.3f, 0.5f, 0.0f);
	glm::vec3 markerPos = boxPos + glm::vec3(0.0f, -0.1f, -0.1f);

	// Render all the sprites
	for (auto it = subEntities.begin(); it != subEntities.end(); it++) {
		(*it)->setPosition(boxPos);

		(*it)->render(spriteShader);

		boxPos.x += xIncrement;
		markerPos.x += xIncrement;
	}

	// Then render the background
	glBindTexture(GL_TEXTURE_2D, texture);

	// Move to position and then move to front
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.0f, 0.0f, 0.1f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PlayerInterfaceGameObject::clean() {
	for (auto it = subEntities.begin(); it != subEntities.end(); it++) {
		(*it)->clean();
	}

	GameObject::clean();
}

GLuint PlayerInterfaceGameObject::backgroundTextureID = 0;

int PlayerInterfaceGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\gui\\main-ui.png");
	backgroundTextureID = textures[offset + 0];
	return offset + 1;
}
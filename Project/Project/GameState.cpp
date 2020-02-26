#include "GameState.h"

void GameState::controls(glm::vec2 mousePos, double deltatime) {
	return;
}

void GameState::update(double deltaTime) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Get the current object
		shared_ptr<GameObject> currentGameObject = *it;

		// Update game objects
		currentGameObject->update(entities, deltaTime);
	}
}

void GameState::render(Shader& shader) {
	// Apply position translate to the view matrix
	glm::mat4 viewMatrix = glm::translate(glm::mat4(), currentViewPosition);

	// Apply zoom scaling to the view matrix
	viewMatrix = glm::scale(viewMatrix, glm::vec3(currentViewZoom, currentViewZoom, currentViewZoom));

	shader.setUniformMat4("viewMatrix", viewMatrix);

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Render game objects
		(*it)->render(shader);
	}
}

void GameState::clean() {
	// Cleanup dirty game objects
	for (auto it = entities.end(); it != entities.begin();) {
		--it;

		// Clean the entity
		(*it)->clean();

		// Delete the current object if it is set to be deleted
		if ((*it)->isDirty()) {
			it = entities.erase(it);
		}
	}
}

int GameState::transtionState() {
	return -1;
}
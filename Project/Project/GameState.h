#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"
#include "Shader.h"
#include "Window.h"

using namespace std;

const GLfloat BASE_GAME_ZOOM = 0.25f;

class GameState {
protected:
	// Current viewport position for this state
	glm::vec3 currentViewPosition;

	// Current viewport zoom for this state
	GLfloat currentViewZoom = BASE_GAME_ZOOM;

	// List of all the entities within this state
	vector<shared_ptr<GameObject>> entities;
public:
	// Method that handles inputs from the user
	virtual void controls(glm::vec2 mousePos, double deltaTime);

	// Method that handles updating all entities within that game state
	virtual void update(double deltaTime);

	// Method that handles rendering all the entities within the game state
	virtual void render(Shader& spriteShader);

	// Method that handles cleaning any dirty state or entities within the game state
	virtual void clean();

	// Method that handles loading that state in the game.
	virtual void load();

	// Utility method that is called at the end of every loop cycle to potentially transition to a new state.
	// Return an integer to transition to a specific state within the game. Return -1 to not transition.
	virtual int transtionState();
};
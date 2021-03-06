#pragma once

#include <memory>
#include <vector>
#include <tuple>

#include "GameObject.h"
#include "Shader.h"
#include "Window.h"

using namespace std;

const GLfloat BASE_GAME_ZOOM = 0.25f;

// Indexs of the various states in the game object states vector. These should be consistent with that vector.
constexpr int MAIN_MENU_STATE = 0;
constexpr int LEVEL_1_STATE = 1;
constexpr int LEVEL_2_STATE = 2;
constexpr int LEVEL_3_STATE = 3;
constexpr int END_GAME_STATE = 4;
constexpr int PAUSE_MENU_STATE = 5;

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
	virtual void controls(glm::vec2 mousePos);

	// Method that handles updating all entities within that game state
	virtual void update(double deltaTime);

	// Method that handles rendering all the entities within the game state
	virtual void render(Shader& spriteShader, Shader& particleShader, Shader& laserShader);

	// Method that handles cleaning any dirty state or entities within the game state
	virtual void clean();

	// Method that handles loading that state in the game.
	virtual void load();

	// Utility method that is called at the end of every loop cycle to potentially transition to a new state.
	// Return a tuple made up of an integer to transition to a specific state within the game. Return -1 to not transition.
	// The second element is a boolean that says if the next state should be reloaded or not.
	virtual tuple<int, bool> transitionState();
};
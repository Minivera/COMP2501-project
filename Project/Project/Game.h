#pragma once

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>

#include "Shader.h"
#include "Window.h"
#include "GameState.h"

using namespace std;

// Base viewport color
const glm::vec3 viewportBackgroundColor(0.1, 0.13, 0.17);

class Game {
private:
	// The game's current State in the state machine
	int currentState;

	// The list of various states available in the state machines
	vector<shared_ptr<GameState>> states;
public:
	// Class constructor that sets the game up with levels and entities
	Game(const GLuint* textures);

	// Main loop method that should run for every update from the main method
	// Returns if the game should still be running, return false to stop it
	bool loop(Window& window, Shader& spriteShader, Shader& particleShader, Shader& laserShader, double deltaTime);
};
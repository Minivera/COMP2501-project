#include "Game.h"
#include "GameState.h"
#include "LevelState.h"

#include "PlayerGameObject.h"
#include "WeaponGameObject.h"

Game::Game(const GLuint* textures) {
	auto player = make_shared<PlayerGameObject>(
		glm::vec3(0.0f, 0.0f, 0.0f),
		PlayerGameObject::playerTextureID,
		WeaponGameObject::harpoonTextureID,
		WeaponGameObject::pistolTextureID,
		WeaponGameObject::laserTextureID,
		WeaponGameObject::bulletTextureID,
		6
	);

	// Set the levels and other states here
	shared_ptr<GameState> level1 = make_shared<LevelState>(0, 1, "Data\\levels\\level1.csv", player);
	shared_ptr<GameState> level2 = make_shared<LevelState>(1, 0, "Data\\levels\\level1.csv", player);

	states.push_back(level1);
	states.push_back(level2);
	currentState = level1;
}

bool Game::loop(Window& window, Shader& spriteShader, Shader& laserShader, double deltaTime) {
	// Clear background
	window.clear(viewportBackgroundColor);

	double posX, posY;
	glfwGetCursorPos(window.getWindow(), &posX, &posY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(Window::getWindow(), &windowWidth, &windowHeight);

	double cursorX = ((posX + 0.5f) / (float)(windowWidth / 2.0f)) - 1.0f;
	double cursorY = 1.0f - ((posY + 0.5f) / (float)(windowHeight / 2.0f));

	// Execute the controls method on the current state
	currentState->controls(glm::vec2(cursorX, cursorY), deltaTime);

	// Update the current state
	currentState->update(deltaTime);

	// Render the current state
	currentState->render(spriteShader, laserShader);

	// Clean anything that needs to be cleaned on the current state
	currentState->clean();

	// Attempt to transition to a new state
	int newState = currentState->transtionState();
	if (newState >= 0) {
		currentState = states.at(newState);
		currentState->load();
	}

	// Update other events like input handling
	glfwPollEvents();

	// Push buffer drawn in the background onto the display
	glfwSwapBuffers(window.getWindow());

	return true;
}
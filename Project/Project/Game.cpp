#include "Game.h"
#include "GameState.h"
#include "LevelState.h"
#include "MainMenuState.h"
#include "PauseMenuState.h"

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
		PlayerGameObject::playerBubblesTextureID,
		6
	);

	// Set the levels and other states here
	shared_ptr<GameState> mainMenu = make_shared<MainMenuState>(player);
	shared_ptr<GameState> level1 = make_shared<LevelState>(LEVEL_1_STATE, LEVEL_2_STATE, "Data\\levels\\level1.csv", player);
	shared_ptr<GameState> level2 = make_shared<LevelState>(LEVEL_2_STATE, LEVEL_3_STATE, "Data\\levels\\level1.csv", player);
	shared_ptr<GameState> level3 = make_shared<LevelState>(LEVEL_3_STATE, END_GAME_STATE, "Data\\levels\\level1.csv", player);
	shared_ptr<GameState> pauseMenu = make_shared<PauseMenuState>(player);

	states.push_back(mainMenu);
	states.push_back(level1);
	states.push_back(level2);
	states.push_back(level3);
	states.push_back(mainMenu); // TODO: Replace with end game screen
	states.push_back(pauseMenu);
	currentState = MAIN_MENU_STATE;
}

bool Game::loop(Window& window, Shader& spriteShader, Shader& particleShader, Shader& laserShader, double deltaTime) {
	// Clear background
	window.clear(viewportBackgroundColor);

	double posX, posY;
	glfwGetCursorPos(window.getWindow(), &posX, &posY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(Window::getWindow(), &windowWidth, &windowHeight);

	double cursorX = ((posX + 0.5f) / (float)(windowWidth / 2.0f)) - 1.0f;
	double cursorY = 1.0f - ((posY + 0.5f) / (float)(windowHeight / 2.0f));

	auto localState = states.at(currentState);

	// Execute the controls method on the current state
	localState->controls(glm::vec2(cursorX, cursorY));

	// Update the current state
	localState->update(deltaTime);

	// Render the current state
	localState->render(spriteShader, particleShader, laserShader);

	// Clean anything that needs to be cleaned on the current state
	localState->clean();

	// Attempt to transition to a new state
	tuple<int, bool> newState = localState->transitionState();
	if (get<0>(newState) >= 0) {
		// Make sure to save the current state in the pause menu.
		if (get<0>(newState) == PAUSE_MENU_STATE) {
			dynamic_pointer_cast<PauseMenuState>(states.at(PAUSE_MENU_STATE))->setPausedState(currentState);
		}

		// Switch the state
		currentState = get<0>(newState);

		// Reload the state if needed.
		if (get<1>(newState)) {
			states.at(currentState)->load();
		}
	}

	// Update other events like input handling
	glfwPollEvents();

	// Push buffer drawn in the background onto the display
	glfwSwapBuffers(window.getWindow());

	return true;
}
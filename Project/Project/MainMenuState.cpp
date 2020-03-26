#include "MainMenuState.h"

#include "ButtonGameObject.h"

MainMenuState::MainMenuState(shared_ptr<PlayerGameObject> player) {
	player = player;

	glm::vec3 buttonScale = glm::vec3(1.6f, 0.6f, 1.0f);

	auto startButton = make_shared<ButtonGameObject>(
		ButtonType::StartButton,
		glm::vec3(-3.0f, -3.5f, 1.0f),
		buttonScale,
		6
	);
	auto quitButton = make_shared<ButtonGameObject>(
		ButtonType::QuitButton,
		glm::vec3(-1.3f, -3.5f, 1.0f),
		buttonScale,
		6
	);

	startButton->onClick(std::bind(&MainMenuState::onStart, this, std::placeholders::_1));
	quitButton->onClick(std::bind(&MainMenuState::onQuit, this, std::placeholders::_1));

	entities.push_back(startButton);
	entities.push_back(quitButton);
}

void MainMenuState::controls(glm::vec2 mousePos, double deltaTime) {
	mousePos.x /= currentViewZoom;
	mousePos.y /= currentViewZoom;

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Run hoevered mechanism on all GUI elements

		auto gui = dynamic_pointer_cast<GuiGameObject>(*it);

		if (gui) {
			gui->verifyMouseOver(mousePos);

			// if we also are clicking the mouse, run the click function
			if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				gui->verifyClick();
			}
		}
	}

	return GameState::controls(mousePos, deltaTime);
}

void MainMenuState::render(Shader& spriteShader, Shader& particleShader, Shader& laserShader) {
	currentViewPosition = glm::vec3(0, 0, 0);

	GameState::render(spriteShader, particleShader, laserShader);

	// Bind the midground texture
	glBindTexture(GL_TEXTURE_2D, backgroundTextureID);

	// Setup the transformation matrix for the shader
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1.1f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(2.0f / currentViewZoom, 2.0f / currentViewZoom, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

tuple<int, bool> MainMenuState::transtionState() {
	if (gameStarted) {
		gameStarted = false;
		return make_tuple(LEVEL_1_STATE, true);
	}

	return GameState::transtionState();
}

void MainMenuState::onStart(GuiGameObject& caller) {
	gameStarted = true;
}

void MainMenuState::onQuit(GuiGameObject& caller) {
	exit(0);
}

GLuint MainMenuState::backgroundTextureID = 0;

int MainMenuState::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\gui\\main-menu.png");
	backgroundTextureID = textures[offset + 0];
	return offset + 1;
}
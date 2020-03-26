#include "PauseMenuState.h"

#include "ButtonGameObject.h"

PauseMenuState::PauseMenuState(shared_ptr<PlayerGameObject> player) {
	player = player;

	glm::vec3 buttonScale = glm::vec3(1.6f, 0.6f, 1.0f);

	auto resumeButton = make_shared<ButtonGameObject>(
		ButtonType::ResumeButton,
		glm::vec3(0.0f, 0.3f, 1.0f),
		buttonScale,
		6
	);
	auto abandonButton = make_shared<ButtonGameObject>(
		ButtonType::AbandonButton,
		glm::vec3(0.0f, -0.3f, 1.0f),
		buttonScale,
		6
	);

	resumeButton->onClick(std::bind(&PauseMenuState::onResume, this, std::placeholders::_1));
	abandonButton->onClick(std::bind(&PauseMenuState::onAbandon, this, std::placeholders::_1));

	entities.push_back(resumeButton);
	entities.push_back(abandonButton);
}

void PauseMenuState::controls(glm::vec2 mousePos, double deltaTime) {
	mousePos.x /= currentViewZoom;
	mousePos.y /= currentViewZoom;

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Run hovered mechanism on all GUI elements

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

void PauseMenuState::render(Shader& spriteShader, Shader& particleShader, Shader& laserShader) {
	currentViewPosition = glm::vec3(0, 0, 0);

	GameState::render(spriteShader, particleShader, laserShader);

	// Bind the midground texture
	glBindTexture(GL_TEXTURE_2D, backgroundTextureID);

	// Setup the transformation matrix for the shader
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1.1f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(3.0f, 6.0f, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

tuple<int, bool> PauseMenuState::transtionState() {
	if (gameResumed) {
		gameResumed = false;
		return make_tuple(pausedState, false);
	}

	if (gameAbandonned) {
		gameAbandonned = false;
		return make_tuple(MAIN_MENU_STATE, false);
	}

	return GameState::transtionState();
}

void PauseMenuState::onResume(GuiGameObject& caller) {
	gameResumed = true;
}

void PauseMenuState::onAbandon(GuiGameObject& caller) {
	gameAbandonned = true;
}

GLuint PauseMenuState::backgroundTextureID = 0;

int PauseMenuState::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\gui\\pause-menu.png");
	backgroundTextureID = textures[offset + 0];
	return offset + 1;
}
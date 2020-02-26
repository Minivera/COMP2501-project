#include "LevelState.h"

#include <GL/glew.h>
#include <GL/glfw3.h>

#include "PlayerGameObject.h"
#include "TerrainGameObject.h"
#include "TreasureGameObject.h"

LevelState::LevelState(const GLuint* textures) {
	backgroundTexture = textures[0];
	midgroundTexture = textures[1];

	int size = 6;

	entities.push_back(make_shared<PlayerGameObject>(glm::vec3(0.0f, 0.0f, 0.0f), textures[2], textures[3], textures[4], textures[5], size));
	entities.push_back(make_shared<TreasureGameObject>(200, glm::vec3(0.5f, 1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 1.0f), textures[10], size));
	entities.push_back(make_shared<TerrainGameObject>(TerrainType::Wall, glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, textures[6], size));
	entities.push_back(make_shared<TerrainGameObject>(TerrainType::Floor, glm::vec3(-0.5f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, textures[8], size));
	entities.push_back(make_shared<TerrainGameObject>(TerrainType::Floor, glm::vec3(0.5f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, textures[9], size));
}

void LevelState::controls(glm::vec2 mousePos, double deltaTime) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));

	double angleToMouse = glm::degrees(glm::atan(mousePos.y, mousePos.x));

	player->setArmRotation(angleToMouse);

	if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
		player->addLeftAcceleration(deltaTime);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
		player->addRightAcceleration(deltaTime);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		player->addLiftAcceleration(deltaTime);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
		player->changeWeapon(WeaponType::Harpoon);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_2) == GLFW_PRESS) {
		// Command for the demo, unlocks the weapon
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			player->unlock(WeaponType::Pistol);
		}
		player->changeWeapon(WeaponType::Pistol);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_3) == GLFW_PRESS) {
		// Command for the demo, unlocks the weapon
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			player->unlock(WeaponType::Laser);
		}
		player->changeWeapon(WeaponType::Laser);
	}
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		player->attack();
	}
}

void LevelState::render(Shader& shader) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));

	currentViewPosition = -player->getPosition() * currentViewZoom;

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GameState::render(shader);

	// Bind the midground texture
	glBindTexture(GL_TEXTURE_2D, midgroundTexture);

	// Setup the transformation matrix for the shader
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), player->getPosition());

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(8.0f, 8.0f, 1.0f));

	// Set the transformation matrix in the shader
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Bind the background texture
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);

	// Setup the transformation matrix for the shader
	transformationMatrix = glm::translate(glm::mat4(), player->getPosition());

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(8.0f, 8.0f, 1.0f));

	// Set the transformation matrix in the shader
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
}
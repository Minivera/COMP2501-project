#include "LevelState.h"

#include <GL/glew.h>
#include <GL/glfw3.h>

#include "FileUtils.h"
#include "TerrainGameObject.h"
#include "ExitGameObject.h"
#include "TreasureGameObject.h"
#include "EnemyGameObject.h"
#include "FishGameObject.h"
#include "JellyfishGameObject.h"

LevelState::LevelState(int levelID, int nextLevelID, const char* levelFile, shared_ptr<PlayerGameObject> player) {
	this->player = player;
	this->levelID = levelID;
	this->nextLevelID = nextLevelID;

	backgroundTexture = backgroundTextureID;
	midgroundTexture = midgroundTextureID;

	levelDefinition = FileUtils::LoadCsvLevel(levelFile, ";");

	loadLevel();
}

void LevelState::loadLevel(bool reloading) {
	int size = 6;

	if (reloading) {
		// If reloading, go through the vector to remove all enemies
		for (auto it = entities.end(); it != entities.begin();) {
			--it;

			// Delete all enemies so they can be respawned
			if (dynamic_pointer_cast<EnemyGameObject>(*it)) {
				it = entities.erase(it);
			}
		}
	}
	else {
		// Else, start the fresh entities vector
		entities.push_back(player);
	}

	int totalRows = levelDefinition.size();
	int totalColumns = levelDefinition.at(0).size();

	double rowStart = (totalRows / 2);
	double columnStart = -(totalColumns / 2);
	double currentRow = rowStart;
	// For each row in the level csv file
	for (auto rowIt = levelDefinition.begin(); rowIt != levelDefinition.end(); rowIt++) {
		double currentColumn = columnStart;

		// For each column in the level CSV file
		for (auto columnIt = (*rowIt).begin(); columnIt != (*rowIt).end(); columnIt++) {
			// Add the given entity to the game
			string val = (*columnIt);
			if (val == NONE) {
				// skip
			}
			else if (val == ENTRY) {
				player->setPosition(glm::vec3(currentColumn, currentRow, 0.0f));
			}
			else if (val == EXIT && !reloading) {
				entities.push_back(make_shared<ExitGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), ExitGameObject::exitTextureID, size));
			}
			else if (val.find(TREASURE) != string::npos && !reloading) {
				string textValue = val.substr(9);
				int treasureValue = stoi(textValue);
				entities.push_back(make_shared<TreasureGameObject>(treasureValue, glm::vec3(currentColumn, currentRow, 0.0f), TreasureGameObject::treasureTextureID, size));
			}
			else if (val == FISH) {
				entities.push_back(make_shared<FishGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), FishGameObject::fishTextureID, size));
			}
			else if (val == JELLYFISH) {
				entities.push_back(make_shared<JellyfishGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), FishGameObject::jellyfishTextureID, size));
			}
			else if (val == SMOKER) {
				// TODO: Change for smoker object when implemented
				entities.push_back(make_shared<JellyfishGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), FishGameObject::smokerTextureID, size));
			}
			else if (val == BG && !reloading) {
				// TODO: Add a BG terrain type and texture
			}
			else if (val == CEILING_1 && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Ceilling,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::ceiling1TextureID,
					size
				));
			}
			else if (val == CEILING_2 && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Ceilling,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::ceiling2TextureID,
					size
				));
			}
			else if (val == FLOOR_1 && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Floor,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::floor1TextureID,
					size
				));
			}
			else if (val == FLOOR_2 && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Floor,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::floor2TextureID,
					size
				));
			}
			else if (val == WALL_L && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn + 0.5, currentRow, 0.0f),
					glm::vec3(0.5f, 1.0f, 1.0f),
					0,
					TerrainGameObject::wallLeftTextureID,
					size
				));
			}
			else if (val == WALL_R && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn - 0.5, currentRow, 0.0f),
					glm::vec3(0.5f, 1.0f, 1.0f),
					0,
					TerrainGameObject::wallRightTextureID,
					size
				));
			}
			else if (val == SLANT_T_L && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn - 0.1, currentRow + 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantTopLeftTextureID,
					size
				));
			}
			else if (val == SLANT_T_R && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn + 0.1, currentRow + 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantTopRightTextureID,
					size
				));
			}
			else if (val == SLANT_B_L && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn - 0.1, currentRow - 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantBottomLeftTextureID,
					size
				));
			}
			else if (val == SLANT_B_R && !reloading) {
				entities.push_back(make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn + 0.1, currentRow - 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantBottomRightTextureID,
					size
				));
			}

			currentColumn += 1;
		}
		currentRow -= 1;
	}
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
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			player->upgrade(WeaponType::Harpoon);
		}
		player->changeWeapon(WeaponType::Harpoon);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_2) == GLFW_PRESS) {
		// Command for the demo, unlocks the weapon
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			player->unlock(WeaponType::Pistol);
		}
		else if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			player->upgrade(WeaponType::Pistol);
		}
		player->changeWeapon(WeaponType::Pistol);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_3) == GLFW_PRESS) {
		// Command for the demo, unlocks the weapon
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			player->unlock(WeaponType::Laser);
		}
		else if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			player->upgrade(WeaponType::Laser);
		}
		player->changeWeapon(WeaponType::Laser);
	}
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		player->attack();
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
		// Demo command to trigger the player getting hurt
		player->hurt();
	}
}

void LevelState::render(Shader& spriteShader) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));

	currentViewPosition = -player->getPosition() * currentViewZoom;
	
	GameState::render(spriteShader);

	// Bind the midground texture
	glBindTexture(GL_TEXTURE_2D, midgroundTexture);
	spriteShader.enable();
	spriteShader.setAttributes();

	// Setup the transformation matrix for the shader
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), player->getPosition());

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(8.0f, 8.0f, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Bind the background texture
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);

	// Setup the transformation matrix for the shader
	transformationMatrix = glm::translate(glm::mat4(), player->getPosition());

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(8.0f, 8.0f, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int LevelState::transtionState() {
	// Check if player collides with the exit
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (dynamic_pointer_cast<ExitGameObject>(*it) && player->checkCollision(*(*it))) {
			return nextLevelID;
		}
	}

	return GameState::transtionState();
}

void LevelState::load() {
	loadLevel(true);
}

GLuint LevelState::backgroundTextureID = 0;
GLuint LevelState::midgroundTextureID = 0;

int LevelState::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\background.png");
	setFuncPtr(textures[offset + 1], "Assets\\environment\\midground.png");
	backgroundTextureID = textures[offset + 0];
	midgroundTextureID = textures[offset + 1];
	return offset + 2;
}
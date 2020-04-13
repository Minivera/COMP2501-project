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
#include "SmokerGameObject.h"
#include "LaserGameObject.h"

LevelState::LevelState(int levelID, int nextLevelID, const char* levelFile, shared_ptr<PlayerGameObject> player) {
	this->player = player;
	this->levelID = levelID;
	this->nextLevelID = nextLevelID;

	backgroundTexture = backgroundTextureID;
	midgroundTexture = midgroundTextureID;

	levelDefinition = FileUtils::LoadCsvLevel(levelFile, ";");

	loadLevel();

	playerInterface = make_shared<PlayerInterfaceGameObject>(
		player,
		glm::vec3(),
		glm::vec3(4.0f, 2.0f, 1.0f),
		PlayerInterfaceGameObject::backgroundTextureID,
		6
	);
	entities.push_back(playerInterface);
}

void LevelState::loadLevel(bool reloading) {
	int size = 6;
	vector<vector<WorldCellDefinition>> worldCells;

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
	for (int row = 0; row < totalRows; row++) {
		double currentColumn = columnStart;

		worldCells.push_back(vector<WorldCellDefinition>());

		// For each column in the level CSV file
		for (int column = 0; column < totalColumns; column++) {
			// Add the given entity to the game
			string val = levelDefinition.at(row).at(column);
			shared_ptr<GameObject> obj;

			// Defines if the current cell is blocking terrain.
			bool blockingTerrain = false;

			if (val == NONE) {
				// skip
			}
			else if (val == ENTRY) {
				player->setPosition(glm::vec3(currentColumn, currentRow, 0.0f));
			}
			else if (val == EXIT && !reloading) {
				obj = make_shared<ExitGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), ExitGameObject::exitTextureID, size);
			}
			else if (val.find(TREASURE) != string::npos && !reloading) {
				string textValue = val.substr(9);
				int treasureValue = stoi(textValue);
				obj = make_shared<TreasureGameObject>(treasureValue, glm::vec3(currentColumn, currentRow, 0.0f), TreasureGameObject::treasureTextureID, size);
			}
			else if (val == FISH) {
				obj = make_shared<FishGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), EnemyGameObject::fishTextureID, size);
			}
			else if (val == JELLYFISH) {
				obj = make_shared<JellyfishGameObject>(glm::vec3(currentColumn, currentRow, 0.0f), EnemyGameObject::jellyfishTextureID, size);
			}
			else if (val == SMOKER) {
				// Find the terrain directly above the current smoker. Since we're going from top to bottom, left to right, we should always have something above us.
				for (int i = row - 1; i >= 0; i--) {
					string current = levelDefinition.at(i).at(column);
					if (current == CEILING_1 || current == CEILING_2 || current == FLOOR_1 || current == FLOOR_2 || current == WALL_L ||
						current == WALL_R || current == SLANT_B_L || current == SLANT_T_L || current == SLANT_B_R || current == SLANT_T_R) {

						auto found = levelMap.find(make_pair(i, column));
						if (found == levelMap.end()) {
							throw "Terrain definition found, but no terrain entity existed for smoker";
						}

						obj = make_shared<SmokerGameObject>(
							dynamic_pointer_cast<TerrainGameObject>(found->second),
							glm::vec3(currentColumn, currentRow, 0.0f),
							EnemyGameObject::smokerTextureID,
							EnemyGameObject::smokerFogTextureID,
							size
						);
						break;
					}
				}

				if (obj == nullptr) {
					throw "No terrain was found above a smoker";
				}
			}
			else if (val == BG && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::BG,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::bgTextureID,
					size
				);
			}
			else if (val == CEILING_1 && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Ceilling,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::ceiling1TextureID,
					size
				);
			}
			else if (val == CEILING_2 && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Ceilling,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::ceiling2TextureID,
					size
				);
			}
			else if (val == FLOOR_1 && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Floor,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::floor1TextureID,
					size
				);
			}
			else if (val == FLOOR_2 && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Floor,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::floor2TextureID,
					size
				);
			}
			else if (val == WALL_L && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::wallLeftTextureID,
					size
				);
			}
			else if (val == WALL_R && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::Wall,
					glm::vec3(currentColumn, currentRow, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					0,
					TerrainGameObject::wallRightTextureID,
					size
				);
			}
			else if (val == SLANT_T_L && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::TopSlant,
					glm::vec3(currentColumn - 0.1, currentRow + 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantTopLeftTextureID,
					size
				);
			}
			else if (val == SLANT_T_R && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::TopSlant,
					glm::vec3(currentColumn + 0.1, currentRow + 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantTopRightTextureID,
					size
				);
			}
			else if (val == SLANT_B_L && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::BottomSlant,
					glm::vec3(currentColumn - 0.1, currentRow - 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantBottomLeftTextureID,
					size
				);
			}
			else if (val == SLANT_B_R && !reloading) {
				blockingTerrain = true;
				obj = make_shared<TerrainGameObject>(
					TerrainType::BottomSlant,
					glm::vec3(currentColumn + 0.1, currentRow - 0.1, 0.0f),
					glm::vec3(0.9f, 0.9f, 1.0f),
					0,
					TerrainGameObject::slantBottomRightTextureID,
					size
				);
			}

			// Insert the object in the entities and map if created
			if (obj != nullptr) {
				entities.push_back(obj);
				levelMap.insert(make_pair(make_pair(row, column), obj));
			}

			worldCells.at(row).push_back(WorldCellDefinition{
				blockingTerrain,
				glm::vec3(currentColumn, currentRow, 0.0f)
			});

			currentColumn += 1;
		}
		currentRow -= 1;
	}

	if (!reloading) {
		// If not reloading, set the game world
		world = make_shared<GameWorld>(worldCells);
	}

	// Set the game world on all enemy entities
	for (auto it = entities.begin(); it != entities.end(); it++) {
		auto enemy = dynamic_pointer_cast<EnemyGameObject>(*it);
		if (enemy) {
			enemy->setWorld(world);
		}
	}
}

void LevelState::controls(glm::vec2 mousePos) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));

	double angleToMouse = glm::degrees(glm::atan(mousePos.y, mousePos.x));

	player->setArmRotation(angleToMouse);

	if (glfwGetKey(Window::getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		pausing = true;
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
		player->addLeftAcceleration();
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
		player->addRightAcceleration();
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		player->addLiftAcceleration();
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
		player->changeWeapon(WeaponType::Harpoon);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_2) == GLFW_PRESS) {
		player->changeWeapon(WeaponType::Pistol);
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_3) == GLFW_PRESS) {
		player->changeWeapon(WeaponType::Laser);
	}
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		player->attack();
	}
}

void LevelState::update(double deltaTime) {
	vector<shared_ptr<GameObject>> currentEntities(entities);

	for (auto it = currentEntities.begin(); it != currentEntities.end(); it++) {
		// Get the current object
		shared_ptr<GameObject> currentGameObject = *it;

		// Check if the entity is within render distance of the player
		if (glm::distance(player->getPosition(), currentGameObject->getPosition()) > renderDistance) {
			// If we're beyond the distance, ignore update
			continue;
		}

		// Update game objects
		currentGameObject->update(entities, deltaTime);
	}
}

void LevelState::render(Shader& spriteShader, Shader& particleShader, Shader& laserShader) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	glm::vec3 playerPos = player->getPosition();

	currentViewPosition = -playerPos * currentViewZoom;

	// Apply position translate to the view matrix
	glm::mat4 viewMatrix = glm::translate(glm::mat4(), currentViewPosition);

	// Apply zoom scaling to the view matrix
	viewMatrix = glm::scale(viewMatrix, glm::vec3(currentViewZoom, currentViewZoom, currentViewZoom));

	playerInterface->setPosition(glm::vec3(playerPos.x + 2.0f, playerPos.y - 4.0f, -2.0f));

	spriteShader.enable();
	spriteShader.setAttributes();
	spriteShader.setUniformMat4("viewMatrix", viewMatrix);

    for (auto it = entities.begin(); it != entities.end(); it++) {
		// Render game objects
		(*it)->render(spriteShader);
	}

	// Bind the midground texture
	glBindTexture(GL_TEXTURE_2D, midgroundTexture);
	spriteShader.setUniformMat4("viewMatrix", viewMatrix);

	// Setup the transformation matrix for the shader
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), glm::vec3(playerPos.x, playerPos.y, 1.1f));

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
	transformationMatrix = glm::translate(glm::mat4(), glm::vec3(playerPos.x, playerPos.y, 1.1f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(8.0f, 8.0f, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	laserShader.enable();
	laserShader.setAttributes();
	laserShader.setUniformMat4("viewMatrix", viewMatrix);

	// Render any special laser object if any
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Render game objects
		auto laser = dynamic_pointer_cast<LaserGameObject>(*it);
		if (laser) {
			laser->renderParticles(laserShader);
		}
	}

	particleShader.enable();
	particleShader.setAttributes();
	particleShader.setUniformMat4("viewMatrix", viewMatrix);

	// Render every other particles
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Render game objects
		auto laser = dynamic_pointer_cast<LaserGameObject>(*it);
		if (!laser) {
			(*it)->renderParticles(particleShader);
		}
	}
}

tuple<int, bool> LevelState::transitionState() {
	if (pausing) {
		pausing = false;
		return make_tuple(PAUSE_MENU_STATE, false);
	}

	// Check if player collides with the exit
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (dynamic_pointer_cast<ExitGameObject>(*it) && player->checkCollision(*(*it))) {
			return make_tuple(nextLevelID, true);
		}
	}

	// Check if the player is out of air
	if (player->getCurrentAir() < 0) {
		return make_tuple(MAIN_MENU_STATE, false);
	}

	return GameState::transitionState();
}

void LevelState::load() {
	loadLevel(true);
}

GLuint LevelState::backgroundTextureID = 0;
GLuint LevelState::midgroundTextureID = 0;

int LevelState::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\background.png");
	setFuncPtr(textures[offset + 1], "Assets\\environment\\midground.png");
	backgroundTextureID = textures[offset + 0];
	midgroundTextureID = textures[offset + 1];
	return offset + 2;
}
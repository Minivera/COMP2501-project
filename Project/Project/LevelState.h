#pragma once

#include <glm/glm.hpp>
#include <map>

#include "GameState.h"

#include "PlayerGameObject.h"
#include "PlayerInterfaceGameObject.h"

constexpr auto NONE = "NONE";
constexpr auto ENTRY = "ENTRY";
constexpr auto EXIT = "EXIT";
constexpr auto TREASURE = "TREASURE_";
constexpr auto FISH = "FISH";
constexpr auto JELLYFISH = "JELLYFISH";
constexpr auto SMOKER = "SMOKER";
constexpr auto BG = "BG";
constexpr auto CEILING_1 = "CEILING_1";
constexpr auto CEILING_2 = "CEILING_2";
constexpr auto FLOOR_1 = "FLOOR_1";
constexpr auto FLOOR_2 = "FLOOR_2";
constexpr auto WALL_L = "WALL_L";
constexpr auto WALL_R = "WALL_R";
constexpr auto SLANT_T_L = "SLANT_T_L";
constexpr auto SLANT_T_R = "SLANT_T_R";
constexpr auto SLANT_B_L = "SLANT_B_L";
constexpr auto SLANT_B_R = "SLANT_B_R";

class LevelState: public GameState {
private:
	// The shared player entity, used to keep it up to date within level.
	shared_ptr<PlayerGameObject> player;

	// Defines the interface for the player that should be displayed at all times.
	shared_ptr<PlayerInterfaceGameObject> playerInterface;

	// The table definition of that level from the fileSystem
	vector<vector<string>> levelDefinition;

	// Map of all the entities in the game and their position in the level definition file
	map<pair<int, int>, shared_ptr<GameObject>> levelMap;

	// The ID of this level in the global state machine.
	int levelID;
	// The ID of the next level in the global state machine.
	int nextLevelID;

	GLuint backgroundTexture = 0;
	GLuint midgroundTexture = 0;

	// Method that will load the level entities in the game state when first loading or when reloading.
	void loadLevel(bool reloading = false);

	// Boolean that says if the game is about to pause in the next loop.
	bool pausing = false;
public:
	LevelState(int levelID, int nextLevelID, const char* levelFile, shared_ptr<PlayerGameObject> player);
	
	void controls(glm::vec2 mousePos);

	void render(Shader& spriteShader, Shader& particleShader, Shader& laserShader);

	void load();

	tuple<int, bool> transitionState();

	// Variables that give the texture IDs for the texture
	static GLuint backgroundTextureID;
	static GLuint midgroundTextureID;

	// Static method to load the background and midground textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset);
};
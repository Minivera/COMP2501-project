#pragma once

#include <glm/glm.hpp>

#include "GameState.h"
#include "PlayerGameObject.h"

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

	// The table definition of that level from the fileSystem
	vector<vector<string>> levelDefinition;

	// The ID of this level in the global state machine.
	int levelID;
	// The ID of the next level in the global state machine.
	int nextLevelID;

	GLuint backgroundTexture = 0;
	GLuint midgroundTexture = 0;

	// Method that will load the level entities in the game state when first loading or when reloading.
	void loadLevel(bool reloading = false);
public:
	LevelState(int levelID, int nextLevelID, const char* levelFile, shared_ptr<PlayerGameObject> player);
	
	void controls(glm::vec2 mousePos, double deltaTime);

	void render(Shader& spriteShader, Shader& particlesShader);

	void load();

	int transtionState();

	// Variables that give the texture IDs for the texture
	static GLuint backgroundTextureID;
	static GLuint midgroundTextureID;

	// Static method to load the background and midground textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};
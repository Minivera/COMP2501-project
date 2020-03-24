#pragma once

#include "GameState.h"

#include <functional>

#include "PlayerGameObject.h"
#include "GuiGameObject.h"

class MainMenuState: public GameState
{
private:
	// The shared player entity, used to keep it up to date within level.
	shared_ptr<PlayerGameObject> player;

	// Class methods that we will link with the various buttons.
	void onStart(GuiGameObject& caller);
	void onQuit(GuiGameObject& caller);

	// Tells the main menu if the game should be starting.
	bool gameStarted = 0;
public:
	MainMenuState(shared_ptr<PlayerGameObject> player);

	void controls(glm::vec2 mousePos, double deltaTime);

	void render(Shader& spriteShader, Shader& particleShader, Shader& laserShader);

	int transtionState();

	// Variables that give the texture IDs for the texture
	static GLuint backgroundTextureID;

	// Static method to load the background and midground textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};


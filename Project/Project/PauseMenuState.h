#pragma once

#include "GameState.h"

#include <functional>

#include "PlayerGameObject.h"
#include "GuiGameObject.h"

class PauseMenuState : public GameState
{
private:
	// The shared player entity, used to keep it up to date within level.
	shared_ptr<PlayerGameObject> player;

	// Class methods that we will link with the various buttons.
	void onResume(GuiGameObject& caller);
	void onAbandon(GuiGameObject& caller);

	// Tells the pause menu what state it should transition to.
	bool gameResumed = false;
	bool gameAbandonned = false;

	// State that was paused when this menu was shown
	int pausedState = 0;
public:
	PauseMenuState(shared_ptr<PlayerGameObject> player);

	void controls(glm::vec2 mousePos, double deltaTime);

	void render(Shader& spriteShader, Shader& particleShader, Shader& laserShader);

	tuple<int, bool> transtionState();

	// Setters
	inline void setPausedState(int state) { pausedState = state; }

	// Variables that give the texture IDs for the texture
	static GLuint backgroundTextureID;

	// Static method to load the background and midground textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};


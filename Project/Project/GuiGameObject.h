#pragma once

#include <functional>

#include "GameObject.h"

class GuiGameObject : public GameObject
{
protected:
	// Check if the current GUI element is hovered by the player.
	bool hovered = false;

	// Check if the current GUI element is clicked by the player.
	bool clicked = false;

	// Set if this element is clickable, true by default. If false, it will not trigger the click event listener.
	bool clickable = true;

	// Timer that will wait a few miliseconds between clicks to prevent the detection of double clicks
	double clickTimer = 0;

	// How long the entity should wait for to register a new click
	const double clickWaitTime = 0.25;

	// Event listeners that will be called when something happens on the entity
	function<void(GuiGameObject & caller)> onClickListener;

	// Textures for the various states
	GLuint hoveredTexture;
	GLuint clickedTexture;

	GuiGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLuint normalTexture, GLuint hoveredTexture, GLuint clickedTexture, GLint entityNumElements);
public:
	// Methods that manage the click and hover system for Gui game elements
	void verifyMouseOver(glm::vec2 mousePos);
	void verifyClick();

	// Overriden update method to manage the click timer.
	void update(std::vector<shared_ptr<GameObject>>& gameObjects, double deltaTime);

	// Overriden render method that will render the gui element
	void render(Shader& spriteShader);
	
	// Overriden clean method that resets the various state variables for hovering and clicking
	void clean();

	// Getters
	inline bool isHovered() const { return hovered; }
	inline bool isClicked() const { return clicked; }

	// Event listeners
	inline void onClick(function<void(GuiGameObject & caller)> onClick) { onClickListener = onClick; }
};


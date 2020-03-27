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

	// Event listeners that will be called when something happens on the entity
	function<void(GuiGameObject & caller)> onClickListener;

	// Textures for the various states
	GLuint hoveredTexture;
	GLuint clickedTexture;

	GuiGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLuint normalTexture, GLuint hoveredTexture, GLuint clickedTexture, GLint entityNumElements);
public:
	void verifyMouseOver(glm::vec2 mousePos);
	void verifyClick();

	void render(Shader& spriteShader);
	
	void clean();

	// Getters
	inline bool isHovered() const { return hovered; }
	inline bool isClicked() const { return clicked; }

	// Event listeners
	inline void onClick(function<void(GuiGameObject & caller)> onClick) { onClickListener = onClick; }
};


#pragma once

#include "GameObject.h"

#include "PlayerGameObject.h"
#include "TreasureCounterGameObject.h"
#include "AirCounterGameObject.h"

class PlayerInterfaceGameObject : public GameObject
{
private:
	// Reference to the player to update the inventory when necessary.
	shared_ptr<PlayerGameObject> player;

	// Reference to the treasure counter in the interface that should update when the players gains more treasure.
	shared_ptr<TreasureCounterGameObject> treasureCounter;

	// Reference to the air counter in the interface that should update when the players gains more base air.
	shared_ptr<AirCounterGameObject> airCounter;

	// Vector with all the entities to render within this entity.
	vector<shared_ptr<GameObject>> subEntities;

	// Method that loads the player interface when constructed;
	void loadInterface();
public:
	PlayerInterfaceGameObject(shared_ptr<PlayerGameObject> player, glm::vec3 entityPos, glm::vec3& entityScale, GLuint entityTexture, GLint entityNumElements);

	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	void render(Shader& spriteShader);

	void clean();

	// Variables that give the texture IDs for the texture
	static GLuint backgroundTextureID;

	// Static method to load the background texture.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset);
};


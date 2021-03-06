#pragma once

#include "EnemyGameObject.h"

#include "TerrainGameObject.h"

class SmokerGameObject: public EnemyGameObject
{
private:
	// Terrain object that is directly above the current element.
	shared_ptr<TerrainGameObject> topTerrain;

	// Texture for the smoke particle.
	GLuint smokeTexture;
public:
	SmokerGameObject(shared_ptr<TerrainGameObject> topTerrain, glm::vec3& entityPos, GLuint entityTexture, GLuint entitySmokeTexture, GLint entityNumElements);

	// Overriden update method to have the smoker generate a wall that the player cannot pass through.
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Overriden render method that will render the smoker on the bottom of its position.
	void render(Shader& spriteShader);

	// Overriden renderParticles method that will render the smoke particles in the air
	void renderParticles(Shader& particlesShader);
	
	// Overriden setWorld setter that will prevent pathfinding on this entity.
	void setWorld(shared_ptr<GameWorld> world);
};


#include "SmokerGameObject.h"

#include "PlayerGameObject.h"
#include "TreasureGameObject.h"
#include "WeaponGameObject.h"
#include "Random.h"

SmokerGameObject::SmokerGameObject(shared_ptr<TerrainGameObject> topTerrain, glm::vec3& entityPos, GLuint entityTexture, GLuint entitySmokeTexture, GLint entityNumElements)
: EnemyGameObject(entityPos, entityTexture, entityNumElements) {
	smokeTexture = entitySmokeTexture;
	this->topTerrain = topTerrain;
	health = 50;
	scale = glm::vec3(0.5, 0.5, 1.0);
	boundingBox = glm::vec2(0.5, 0.5);
}

void SmokerGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	entityTime += deltaTime;

	// Check if we have a player near our smoke
	auto player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	if (player && seesEntity(topTerrain->getPosition(), *player)) {
		// If we do, hurt the player
		player->hurt();
	}

	// Attemps to collide with something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a weapon of some kind
		auto weapon = dynamic_pointer_cast<WeaponGameObject>(*it);
		if (currentState != EnemyState::DIE && currentState != EnemyState::HURT && currentState != EnemyState::HURTING && weapon && checkCollision(*(*it))) {
			hurt(weapon->getDamage());
			weapon->setDirty(true);
			break;
		}
	}

	// Handle hurting mechanism
	if (currentState == EnemyState::HURT) {
		// We processed the hurt state, now we're executing it.
		currentState = EnemyState::HURTING;
	}
	else if (currentState == EnemyState::HURTING) {
		invicibilityTimer -= deltaTime;
	}

	if (currentState == EnemyState::DIE) {
		// Drop treasure and set as dirty
		dirty = true;
		entities.push_back(make_shared<TreasureGameObject>(random::randomInt(1, 3) * 5, position + glm::vec3(0, -0.3, 0), TreasureGameObject::treasureTextureID, numElements));
	}
}

void SmokerGameObject::render(Shader& spriteShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);
	spriteShader.enable();
	spriteShader.setAttributes();

	spriteShader.setUniform1f("count", 0);
	spriteShader.setUniform1f("time", entityTime);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then move the entity down on the ground
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0, -0.5f, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	if (currentState == EnemyState::HURTING && random::randomBool()) {
		spriteShader.setUniform4f("objectColor", glm::vec4(1, 0, 0, 0.6));
	}
	else {
		spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	}

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void SmokerGameObject::renderParticles(Shader& particlesShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, smokeTexture);
	particlesShader.enable();
	particlesShader.setAttributes();

	auto otherPos = topTerrain->getPosition();
	auto otherBox = topTerrain->getBoundingBox();
	glm::vec3 boundPosition = glm::vec3(
		otherPos.x + otherBox.x,
		otherPos.y - otherBox.y,
		otherPos.z
	);
	double distance = glm::distance(position, boundPosition);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Move forward so the particles are rendered above everything
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0, 0, -1.0f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5f, 0.5f, 1.0f));

	particlesShader.setUniform1f("count", 6.0f);
	particlesShader.setUniform1f("time", entityTime);
	particlesShader.setUniform1f("distance", distance * 2);

	// Set the transformation matrix in the shader
	particlesShader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements * distance * 6, GL_UNSIGNED_INT, 0);
}
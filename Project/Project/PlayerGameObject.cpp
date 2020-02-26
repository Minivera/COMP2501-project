#include "PlayerGameObject.h"

#include "TerrainGameObject.h"
#include "TreasureGameObject.h"
#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint entityHarpoonTexture, GLuint entityPistolTexture, GLuint entityLaserTexture, GLint entityNumElements)
	: GravityGameObject(entityPos, entityTexture, entityNumElements) {
	harpoonTexture = entityHarpoonTexture;
	pistolTexture = entityPistolTexture;
	laserTexture = entityLaserTexture;
}

void PlayerGameObject::update(std::vector<shared_ptr<GameObject>> entities, double deltaTime) {
	currentAir -= deltaTime;

	// Reload the current weapon from the inventory
	inventory->getEquipedWeapon().recharge(deltaTime);
	
	// Handle attack mechanism
	if (currentState == PlayerState::ATTACK) {
		// We processed the attack state, now we're executing it.
		currentState = PlayerState::ATTACKING;
		inventory->getEquipedWeapon().attack(position, armRotation, entities);

		armMovement = inventory->getEquipedWeapon().getRechargeRate();
	}
	else if (currentState == PlayerState::ATTACKING) {
		armMovement -= deltaTime;
		if (armMovement <= 0) {
			armMovement = 0;
			currentState = PlayerState::NONE;
		}
	}

	shared_ptr<TerrainGameObject> collidesWith;

	// Attemps to collide with a something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a wall
		auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (terrain && checkCollision(*(*it))) {
			collidesWith = terrain;
		}

		auto treasure = dynamic_pointer_cast<TreasureGameObject>(*it);
		if (treasure && checkCollision(*(*it))) {
			currentTreasure += treasure->getValue();
			treasure->setDirty(true);
		}
	}
	
	if (collidesWith != nullptr && collidesWith->getType() == TerrainType::Wall) {
		// If colliding with a wall, deactivate gravity, stop any movement and slowly move down
		gravityActivated = false;
		velocity.y = glm::max(-maxSpeed * wallSlowEffect * deltaTime, velocity.y - slowDownFactor.y / wallSlowEffect);
		// Prevent movement on the side we collided with the wall
		if (velocity.x > 0 && getCollisionSide(*collidesWith) > 0) {
			velocity.x = 0;
		} else if (velocity.x < 0 && getCollisionSide(*collidesWith) < 0) {
			velocity.x = 0;
		}
	}
	else if (collidesWith != nullptr && collidesWith->getType() == TerrainType::Floor) {
		// If colliding with the floor, slow down movement
		velocity.x *= wallSlowEffect;
	}
	else if (collidesWith != nullptr && collidesWith->getType() == TerrainType::Ceilling) {
		// If colliding with the ceiling, slow down movement and stop lift
		velocity.y = 0;
		velocity.x *= wallSlowEffect;
	}

	// Slow the entity based on its slow down factor
	if (collidesWith == nullptr && velocity.x > 0 && !moved) {
		velocity.x = glm::max(0.0f, velocity.x - slowDownFactor.x);
	} else if (collidesWith == nullptr && velocity.x < 0 && !moved) {
		velocity.x = glm::min(0.0f, velocity.x - slowDownFactor.x);
	}

	if (collidesWith == nullptr && velocity.y > 0 && !moved) {
		velocity.y = glm::max(0.0f, velocity.y - slowDownFactor.y);
	}

	GravityGameObject::update(entities, deltaTime);
}

// Renders the PlayerGameObject using the shader and then renders their weapon
void PlayerGameObject::render(Shader& shader) {
	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	glm::mat4 weaponMatrix;
	switch (inventory->getEquipedWeaponType()) {
	case WeaponType::Harpoon:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, harpoonTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.5, -0.5, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.5, 0.5, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation - 90.0f, glm::vec3(0, 0, 1.0f));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.5, 0.75, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, armMovement, 0));

		// Set the transformation matrix in the shader
		shader.setUniformMat4("transformationMatrix", weaponMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		break;
	case WeaponType::Pistol:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, pistolTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0.2, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.2, 0, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.2, 0, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation - 90.0f, glm::vec3(0, 0, 1.0f));

		// Bring back the offset after rotation
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, 0.1f, 0));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.15, 0.30, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, -armMovement, 0));

		// Set the transformation matrix in the shader
		shader.setUniformMat4("transformationMatrix", weaponMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		break;
	case WeaponType::Laser:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, laserTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0.2, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.2, 0, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.2, 0, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation - 90.0f, glm::vec3(0, 0, 1.0f));

		// Bring back the offset after rotation
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, 0.1f, 0));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.30, 0.60, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, -armMovement * 0.25, 0));

		// Set the transformation matrix in the shader
		shader.setUniformMat4("transformationMatrix", weaponMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		break;
	}

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the transformation matrix in the shader
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void PlayerGameObject::clean() {
	slowDownFactor = glm::vec2();
	moved = false;
}

void PlayerGameObject::addLiftAcceleration(double deltaTime) {
	double movement = baseAcceleration * deltaTime;
	velocity = glm::vec3(velocity.x, glm::min(velocity.y + movement, maxSpeed * deltaTime), velocity.z);
	slowDownFactor.y = velocity.y / 50;
	moved = true;
}

void PlayerGameObject::addLeftAcceleration(double deltaTime) {
	double movement = baseAcceleration * deltaTime;
	velocity = glm::vec3(glm::max(velocity.x - movement, -(maxSpeed * deltaTime)), velocity.y, velocity.z);
	slowDownFactor.x = velocity.x / 50;
	moved = true;
}

void PlayerGameObject::addRightAcceleration(double deltaTime) {
	double movement = baseAcceleration * deltaTime;
	velocity = glm::vec3(glm::min(velocity.x + movement, maxSpeed * deltaTime), velocity.y, velocity.z);
	slowDownFactor.x = velocity.x / 50;
	moved = true;
}

void PlayerGameObject::changeWeapon(WeaponType type) {
	if (currentState == PlayerState::NONE) {
		inventory->equip(type);
	}
}

void PlayerGameObject::unlock(WeaponType type) {
	if (currentState == PlayerState::NONE) {
		inventory->unlock(type);
	}
}

void PlayerGameObject::upgrade(WeaponType type) {
	if (currentState == PlayerState::NONE) {
		inventory->upgrade(type);
	}
}

void PlayerGameObject::attack() {
	if (currentState == PlayerState::NONE) {
		currentState = PlayerState::ATTACK;
	}
}
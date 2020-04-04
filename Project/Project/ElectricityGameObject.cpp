#include "ElectricityGameObject.h"

#include "TerrainGameObject.h"
#include "PlayerGameObject.h"

ElectricityGameObject::ElectricityGameObject(double entityLifespan, glm::vec3& entitySpeed, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entitySpeed, glm::vec3(0.25, 0.25, 1), entityAngle, entityTexture, entityNumElements) {
	lifespan = entityLifespan;
	angle = entityAngle;
	speed = entitySpeed;
}

void ElectricityGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	auto player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));

	if (player && checkCollision(*player)) {
		player->hurt(*this);
		dirty = true;
		return;
	}

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a wall
		auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (terrain && checkCollision(*(*it))) {
			dirty = true;
			return;
		}
	}

	lifespan -= deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	velocity.x = speed.x;

	GameObject::update(entities, deltaTime);
}

GLuint ElectricityGameObject::projectileTextureID;

int ElectricityGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\projectile.png");
	projectileTextureID = textures[offset + 0];
	return offset + 1;
}

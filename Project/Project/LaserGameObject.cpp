#include "LaserGameObject.h"

#include "LineOfSight.h"
#include "EnemyGameObject.h"

LaserGameObject::LaserGameObject(int weaponDamage, double entityLifespan, GLfloat entitySize, glm::vec3& entityPosition, GLfloat entityAngle) :
	WeaponGameObject(weaponDamage, entityPosition, glm::vec3(), glm::vec3(), entityAngle, 0, 0) {
	baseLifespan = entityLifespan;
	lifespan = entityLifespan;
	angle = entityAngle;
	size = entitySize;
}

void LaserGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// The laser acts like a line of sight. If we see something, they we can hit it.
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a weapon of some kind
		auto enemy = dynamic_pointer_cast<EnemyGameObject>(*it);
		if (enemy && checkLaserHits(*enemy)) {
			enemy->hurt(damage);
		}
	}

	lifespan -= deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	GameObject::update(entities, deltaTime);
}

bool LaserGameObject::checkLaserHits(GameObject& entity) {
	glm::vec3 selfPos = position;
	glm::vec3 otherPos = entity.getPosition();
	glm::vec2 otherBoundingBox = entity.getBoundingBox();

	glm::vec3 sightLength = glm::vec3(
		cos(glm::radians(angle)) * 10,
		sin(glm::radians(angle)) * 10,
		0
	);

	// The laser width is a vector that add half the size perpendicular to the laser's angle
	glm::vec3 laserWidth = glm::vec3(
		cos(angle) + glm::radians(90.0f) * size / 2,
		sin(angle) + glm::radians(90.0f) * size / 2,
		0
	);

	auto posLaserLeft = selfPos - laserWidth;
	auto posLaserRight = selfPos + laserWidth;

	// Draw three lines of sight depending on the laser size, if we are on one or the other, there is a hit.
	auto lineOfSightLeft = LineOfSight::drawLine(posLaserLeft.x , posLaserLeft.y, sightLength.x, sightLength.y);
	auto lineOfSightCenter = LineOfSight::drawLine(selfPos.x, selfPos.y, sightLength.x, sightLength.y);
	auto lineOfSightRight = LineOfSight::drawLine(posLaserRight.x, posLaserRight.y, sightLength.x, sightLength.y);
	auto entityRectangle = LineOfSight::drawRectangle(
		otherPos.x - otherBoundingBox.x / 2,
		otherPos.y + otherBoundingBox.y / 2,
		otherPos.x + otherBoundingBox.x / 2,
		otherPos.y - otherBoundingBox.y / 2
	);

	return LineOfSight::intersectsWith(lineOfSightLeft, entityRectangle)
		|| LineOfSight::intersectsWith(lineOfSightCenter, entityRectangle)
		|| LineOfSight::intersectsWith(lineOfSightRight, entityRectangle);
}

void LaserGameObject::render(Shader& spriteShader) {}

void LaserGameObject::renderParticles(Shader& particlesShader) {
	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), position);

	// Move forward so the particles are rendered above everything
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0, 0, -1.0f));

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));

	// Add an offset so the laser is rescaled from that offset
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.9f, 0, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(10.0f, 1.0f, 1.0f));

	// Attach the laser to the left of its position
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.6, 0, 0));

	// Set the transformation matrix in the shader
	particlesShader.setUniformMat4("transformationMatrix", transformationMatrix);
	particlesShader.setUniform1f("size", 6.0f * size);
	particlesShader.setUniform1f("lifespanLeft", lifespan);
	particlesShader.setUniform1f("lifespan", baseLifespan);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

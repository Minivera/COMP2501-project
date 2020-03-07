#include "LaserGameObject.h"

LaserGameObject::LaserGameObject(int weaponDamage, double entityLifespan, GLfloat entitySize, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements) :
	WeaponGameObject(weaponDamage, entityPosition, glm::vec3(), glm::vec3(4, entitySize, 1), entityAngle, entityTexture, entityNumElements) {
	lifespan = entityLifespan;
	angle = entityAngle;
	size = entitySize;
}

void LaserGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	lifespan -= deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	GameObject::update(entities, deltaTime);
}

void LaserGameObject::render(Shader& spriteShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);
	spriteShader.enable();
	spriteShader.setAttributes();

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));

	// Attach the laser to the left of its position
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(2.3, 0, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

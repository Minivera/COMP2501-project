#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements):
 GameObject(entityPosition, entityTexture, entityNumElements) {
	damage = weaponDamage;
}

WeaponGameObject::WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {
	damage = weaponDamage;
}

WeaponGameObject::WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation,
	GLuint entityTexture, GLint entityNumElements): GameObject(entityPosition, entityVelocity, entityScale, entityRotation, entityTexture, entityNumElements) {
	damage = weaponDamage;
}

int WeaponGameObject::setWeaponTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\trident-single.png");
	setFuncPtr(textures[offset + 1], "Assets\\objects\\pistol.png");
	setFuncPtr(textures[offset + 2], "Assets\\objects\\laser.png");
	setFuncPtr(textures[offset + 3], "Assets\\objects\\bullet.png");
	setFuncPtr(textures[offset + 4], "Assets\\objects\\laser_ray.png");
	return offset + 5;
}
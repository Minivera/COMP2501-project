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

GLuint WeaponGameObject::harpoonTextureID;
GLuint WeaponGameObject::pistolTextureID;
GLuint WeaponGameObject::laserTextureID;
GLuint WeaponGameObject::bulletTextureID;
GLuint WeaponGameObject::laserRayTextureID;

int WeaponGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\trident-single.png");
	setFuncPtr(textures[offset + 1], "Assets\\objects\\pistol.png");
	setFuncPtr(textures[offset + 2], "Assets\\objects\\laser.png");
	setFuncPtr(textures[offset + 3], "Assets\\objects\\bullet.png");
	setFuncPtr(textures[offset + 4], "Assets\\objects\\laser_ray.png");
	harpoonTextureID = textures[offset + 0];
	pistolTextureID = textures[offset + 1];
	laserTextureID = textures[offset + 2];
	bulletTextureID = textures[offset + 3];
	laserRayTextureID = textures[offset + 4];
	return offset + 5;
}
#pragma once

#include "GameObject.h"

class WeaponGameObject: public GameObject
{
protected:
	// The amount of damage this class causes
	int damage = 0;

	WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);
	WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
	WeaponGameObject(int weaponDamage, glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
public:
	// Getters
	inline int getDamage() { return damage; }

	// Static method to load the weapon textures from the file system
	static int setWeaponTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};


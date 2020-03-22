#include "PowerupGameObject.h"

PowerupGameObject::PowerupGameObject(glm::vec3& entityPosition, GLint entityNumElements) : 
	GravityGameObject(entityPosition, glm::vec3(0.3f, 0.3f, 1.0f), 0, 0, entityNumElements) {
	dataStruct = powerup::spawn();

	if (dataStruct.type == PowerupType::Armor) {
		texture = armorTextureID;
	} else if (dataStruct.type == PowerupType::Air) {
		texture = airTextureID;
	} else if (dataStruct.type == PowerupType::Pepper) {
		texture = pepperTextureID;
	}
}

GLuint PowerupGameObject::armorTextureID = 0;
GLuint PowerupGameObject::airTextureID = 0;
GLuint PowerupGameObject::pepperTextureID = 0;

int PowerupGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\armor.png");
	setFuncPtr(textures[offset + 1], "Assets\\objects\\air.png");
	setFuncPtr(textures[offset + 2], "Assets\\objects\\pepper.png");
	armorTextureID = textures[offset + 0];
	airTextureID = textures[offset + 1];
	pepperTextureID = textures[offset + 2];
	return offset + 3;
}
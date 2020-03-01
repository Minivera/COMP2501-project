#pragma once

#include "GameObject.h"

// Possible states for the enemy state machine. The fact they cannot attack and be hurt at the same time is as designed.
enum class EnemyState { IDLE, PATH, PATHING, CHASE, CHASING, FLEE, FLEEING, HURT, HURTING, ATTACK, ATTACKING, DIE };

class EnemyGameObject: public GameObject
{
protected:
	EnemyState currentState = EnemyState::IDLE;

	// Max speed of the base enemy.
	const GLfloat maxSpeed = 45.0;

	// The length of time an enemy will stay invicible for.
	const double invicibleTime = 0.5;

	// Timer that counts how long a enemy stays invicible
	double invicibilityTimer = 0;

	// The enemy health meter
	int health = 0;

	EnemyGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);
public:
	// Overriden update method to update the enemy and handle collisions
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Overriden render method to render the enemy and make it flash when hurt.
	virtual void render(Shader& shader);

	// Overriden clean method to clean anything the enemy needs cleaning.
	virtual void clean();

	// Method to trigger an enemy's attack.
	virtual void attack();

	// Method to damage an enemy.
	virtual void hurt(int damage);

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint fishTextureID;
	static GLuint jellyfishTextureID;
	static GLuint smokerTextureID;

	// Static method to load the various enemy sprites from the filesystem
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};


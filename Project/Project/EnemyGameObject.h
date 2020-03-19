#pragma once

#include "GameObject.h"

// Possible states for the enemy state machine. The fact they cannot attack and be hurt at the same time is as designed.
enum class EnemyState { IDLE, CHASE, CHASING, FLEE, FLEEING, HURT, HURTING, ATTACK, ATTACKING, COLLIDE, COLLIDING, DIE };

class EnemyGameObject: public GameObject
{
protected:
	GLfloat angleToCollision = 0;

	// Counter that counts how long the player has lived. Useful for animation and things like that.
	GLfloat entityTime = 0.0;

	// The enemy state machine
	EnemyState currentState = EnemyState::IDLE;

	// Max speed of the base enemy.
	const GLfloat maxSpeed = 45.0;

	// The length of time an enemy will stay invicible for.
	const double invicibleTime = 0.95;

	// Timer that counts how long a enemy stays invicible
	double invicibilityTimer = 0;

	// The length of time an enemy will move when colliding for.
	const double collisionTime = 0.95;

	// Timer that counts how long a enemy stays in the colliding state
	double collisonTimer = 0;

	// The enemy health meter
	int health = 0;

	EnemyGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);

	virtual bool seesEntity(const glm::vec3& direction, const GameObject& other);
public:
	// Overriden update method to update the enemy and handle collisions
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Overriden render method to render the enemy and make it flash when hurt.
	virtual void render(Shader& spriteShader);

	// Overriden clean method to clean anything the enemy needs cleaning.
	virtual void clean();

	// Method to trigger an enemy's attack.
	virtual void attack();

	// Method to damage an enemy.
	virtual void hurt(int damage);

	// Method to cause a collision with something in the game world.
	virtual void collide();

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint fishTextureID;
	static GLuint jellyfishTextureID;
	static GLuint smokerTextureID;

	// Static method to load the various enemy sprites from the filesystem
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};


#pragma once

#include "GravityGameObject.h"
#include "PlayerInventory.h"

// Possible states for the player state machine.
// TODO: Make sure we can both attack and be hurt at the same time.
enum class PlayerState { NONE, HURT, HURTING, ATTACK, ATTACKING };

// Inherits from GameObject
class PlayerGameObject : public GravityGameObject {
private:
	// Attribute that command the slow down of the player when moving
	glm::vec3 collidedVelicity;

	// Counter that counts how long the player has lived. Useful for animation and things like that.
	GLfloat playerTime = 0.0;

	// The player state machine
	// FIXME: Currently it does not allow shooting and hurting at the same time.
	PlayerState currentState = PlayerState::NONE;

	// Maximum speed of the player in units per milliseconds
	double maxSpeed = 2.0;

	// The base acceleration of the player in units per milliseconds
	const double baseAcceleration = 0.15;

	// How much the player gains in speed per upgrade.
	const double speedIncrease = 0.10;

	// The slow down effect on the acceleration from colliding with terrain
	const double wallSlowEffect = 0.65;

	// The length of time a player will stay invicible for.
	const double invicibleTime = 0.95;

	// Holds the current player's inventory of weapons and upgrades.
	unique_ptr<PlayerInventory> inventory;

	// Timer that counts how long a player stays invicible
	double invicibilityTimer = 0;

	// holds the current arm rotation of the player based on the mouse position.
	GLfloat armRotation = 0;
	double armMovement = 0;

	// Textures for the three weapons
	GLuint harpoonTexture;
	GLuint pistolTexture;
	GLuint laserTexture;

	// Texture for the bubbles particle
	GLuint bubblesTexture;
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint entityHarpoonTexture, GLuint entityPistolTexture,
		GLuint entityLaserTexture, GLuint entityBulletTexture, GLuint entityBubblesTexture, GLint entityNumElements);

	// Overriden update method to update the player and handle collisions
	void update(std::vector<shared_ptr<GameObject>>& gameObjects, double deltaTime);

	// Overriden render emthod to render both the player and the weapon they are carrying
	void render(Shader& spriteShader);

	// Overriden renderParticles method that will render some bubbles above the player
	void renderParticles(Shader& particlesShader);

	// Overriden clean method to clean anything the player needs cleaning.
	void clean();

	// Method that adds lift acceleration to the player so they can fight gravity.
	void addLiftAcceleration();

	// Two methods that enable the player to gain momentum going left or right.
	void addLeftAcceleration();
	void addRightAcceleration();

	// Inventory management methods
	void changeWeapon(WeaponType type);
	bool unlock(WeaponType type);
	bool upgrade(UpgradeType type);

	// Method to trigger a player's attack cycle, which will be executed during the next update cycle.
	void attack();

	// Method to trigger a player being hurt by something in the environment.
	void hurt(GameObject& enemy);

	// Method that makes the player pick up something.
	void pickUp(shared_ptr<TreasureGameObject> treasure);
	void pickUp(shared_ptr<PowerupGameObject> powerup);

	// Getters
	inline int getCurrentTreasure() const { return inventory->getTreasure(); }
	inline double getBaseAir() const { return inventory->getBaseAir(); }
	inline double getCurrentAir() const { return inventory->getAir(); }

	// Upgrade related getters
	inline int getAirLevel() const { return inventory->getAirLevel(); }
	inline int getSuitLevel() const { return inventory->getSuitLevel(); }
	inline int getFlippersLevel() const { return inventory->getFlippersLevel(); }

	// Setters
 	inline void setArmRotation(double newRotation) { armRotation = newRotation; }

	// Variables that give the texture IDs for the textures of this entity
	static GLuint playerTextureID;
	static GLuint playerMovingTextureID;
	static GLuint playerHurtTextureID;
	static GLuint playerBubblesTextureID;
	static GLuint playerFlippersTextureID;

	// Static method to load the player textures.
	static int setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset);
};

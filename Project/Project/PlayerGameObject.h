#pragma once

#include "GravityGameObject.h"
#include "PlayerInventory.h"

// Possible states for the player state machine.
enum class PlayerState { NONE, HURT, ATTACK, ATTACKING };

// Inherits from GameObject
class PlayerGameObject : public GravityGameObject {
private:
	// Attribute that command the slow down of the player when moving
	bool moved = false;
	glm::vec2 slowDownFactor;

	PlayerState currentState = PlayerState::NONE;

	// Maximum speed of the player in units per milliseconds
	const double maxSpeed = 50.0;

	// The base acceleration of the player in units per milliseconds
	const double baseAcceleration = 5.0;

	// The slow down effect on teh acceleration from colliding with terrain
	const double wallSlowEffect = 0.65;

	// Attribute that counts how much treasure the player has available.
	int currentTreasure = 0;

	// Attribute that counts the current air from the player in seconds.
	double currentAir = 0;

	// Holds the current player's inventory of weapons and upgrades.
	unique_ptr<PlayerInventory> inventory = make_unique<PlayerInventory>();

	// holds the current arm rotation of the player based on the mouse position.
	GLfloat armRotation = 0;
	double armMovement = 0;

	// Textures for the three weapons
	GLuint harpoonTexture;
	GLuint pistolTexture;
	GLuint laserTexture;
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint entityHarpoonTexture, GLuint entityPistolTexture, GLuint entityLaserTexture, GLint entityNumElements);

	// Overriden update method to update the player and handle collisions
	void update(std::vector<shared_ptr<GameObject>> gameObjects, double deltaTime);

	// Overriden render emthod to render both the player and the weapon they are carrying
	void render(Shader& shader);

	// Overriden clean method to clean anything the player needs cleaning.
	void clean();

	// Method that adds lift acceleration to the player so they can fight gravity.
	void addLiftAcceleration(double deltaTime);

	// Two methods that enable the player to gain momentum going left or right.
	void addLeftAcceleration(double deltaTime);
	void addRightAcceleration(double deltaTime);

	// Inventory management methods
	void changeWeapon(WeaponType type);
	void unlock(WeaponType type);
	void upgrade(WeaponType type);

	// Method to trigger a player's attack cycle, which will be executed during the next update cycle.
	void attack();

	// Getters
	inline int getCurrentTreasure() { return currentTreasure; }
	inline double getCurrentAir() { return currentAir; }

	// Setters
	inline void setCurrentTreasure(int newTreasure) { currentTreasure = newTreasure; }
	inline void setCurrenAir(double newAir) { currentAir = newAir; }
	inline void setArmRotation(double newRotation) { armRotation = newRotation; }
};

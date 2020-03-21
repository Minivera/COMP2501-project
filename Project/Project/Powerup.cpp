#include "Powerup.h"

#include "Random.h"

namespace powerup {
	Powerup& spawn() {
		// Choose which type of powerup to spawn
		switch (random::randomInt(0, 2))
		{
		case 0:
			return Powerup{
				PowerupType::Armor
			};
		case 1:
			return Powerup{
				PowerupType::Air
			};
		case 2:
			return Powerup{
				PowerupType::Pepper
			};
		default:
			throw "Powerup type out of range";
		}
	}
}
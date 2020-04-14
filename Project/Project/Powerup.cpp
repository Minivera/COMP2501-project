#include "Powerup.h"

#include "Random.h"

namespace powerup {
	Powerup& spawn() {
		// Choose which type of powerup to spawn
		switch (random::randomInt(0, 2))
		{
		case 0:
			return Powerup{
				PowerupType::Armor,
				5,
				0 // Always on
			};
		case 1:
			return Powerup{
				PowerupType::Air,
				0,
				30 // will add 30 seconds of air back
			};
		case 2:
			return Powerup{
				PowerupType::Pepper,
				15,
				0.5 //boost damage by 50%
			};
		default:
			throw "Powerup type out of range";
		}
	}
}
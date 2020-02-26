#include "Random.h"

namespace random {
	float randomFloat(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;

		float range = max - min;
		return (random * range) + min;
	}

	int randomInt(int min, int max) {
		return min + rand() % ((max + 1) - min);
	}

	bool randomBool() {
		if (rand() % 2 == 0) {
			return true;
		}
		return false;
	}
}
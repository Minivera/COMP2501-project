// galik.h
#ifndef RANDOM_NAMESPACE
#define RANDOM_NAMESPACE

#include <stdlib.h>

using namespace std;

namespace random {
	float randomFloat(float min, float max);
	int randomInt(int min, int max);
	bool randomBool();
}

#endif
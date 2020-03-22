#pragma once

enum class PowerupType { Armor, Air, Pepper };

struct Powerup {
	PowerupType type;
	double duration;
	double effect;
};

namespace powerup
{
	Powerup& spawn();
};


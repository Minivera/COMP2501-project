#pragma once

enum class PowerupType { Armor, Air, Pepper };

struct Powerup {
	PowerupType type;
};

namespace powerup
{
	static Powerup& spawn();
};


#include "AlienAttack.h"

AlienAttack::AlienAttack()
{
    alienModeActive = false;
    chance = 0.375; // default 37.5% chance

    // Seed RNG with chrono for better randomness
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void AlienAttack::tryTrigger()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    const double roll = dist(rng);
    alienModeActive = (roll < chance);
}

bool AlienAttack::isActive() const
{
    return alienModeActive;
}

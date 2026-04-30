#pragma once
#include <random>
#include <chrono>

class AlienAttack
{
private:
    bool alienModeActive; // false = normal, true = reversed
    double chance;        // 0.35–0.40

    std::mt19937 rng;

public:
    AlienAttack();

    // Attempt to trigger an alien attack based on random chance
    void tryTrigger();

    // Returns whether alien mode is currently active
    bool isActive() const;
};

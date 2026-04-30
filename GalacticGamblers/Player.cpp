#include "Player.h"
#include <cstring>
#include <iostream>

Player::Player(std::string playerName)
{
    name = playerName;
    shipName[0] = '\0';
    totalScore = 0;
    multiplier = 1;
    roundPoints = 0;
    targetScore = 0;
}

void Player::setShipName(const char* ship)
{
    strncpy_s(shipName, ship, sizeof(shipName) - 1);
    shipName[sizeof(shipName) - 1] = '\0';
}

std::string Player::getName() const
{
    return name;
}

std::string Player::reverseRecursive(const std::string& text, int index) const
{
    if (index < 0)
        return "";

    return text[static_cast<size_t>(index)] + reverseRecursive(text, index - 1);
}

std::string Player::getDisplayName(bool alienReversal) const
{
    if (!alienReversal)
        return name;

    return reverseRecursive(name, static_cast<int>(name.size()) - 1);
}

std::string Player::getDisplayShipName(bool alienReversal) const
{
    const std::string ship(shipName);
    if (!alienReversal)
        return ship;

    return reverseRecursive(ship, static_cast<int>(ship.size()) - 1);
}

int Player::getTotalScore() const
{
    return totalScore;
}

void Player::startRaid(int target)
{
    targetScore = target;
    roundPoints = 0;
    totalScore = 0;
    multiplier = 1;
}

void Player::addHandScore(int points)
{
    roundPoints += points;
}

void Player::commitRoundScore()
{
    totalScore += roundPoints;
}

bool Player::raidSuccess() const
{
    return roundPoints >= targetScore;
}

int Player::recursiveMultiplierBonus(int overflow)
{
    if (overflow <= 0)
        return 0;

    const int bonus = overflow * multiplier;
    return bonus + recursiveMultiplierBonus(overflow - 1);
}

void Player::applyOverflowBonus()
{
    const int overflow = roundPoints - targetScore;
    if (overflow > 0)
    {
        const int bonus = recursiveMultiplierBonus(overflow);
        totalScore += bonus;
        multiplier++;
    }
}

void Player::resetTotalScore()
{
    totalScore = 0;
    roundPoints = 0;
    targetScore = 0;
    multiplier = 1;
}

void Player::addArtifactPower(int power)
{
    artifactPowers.push(power);
}

void Player::useArtifactPower()
{
    if (!artifactPowers.empty())
    {
        const int power = artifactPowers.top();
        artifactPowers.pop();
        std::cout << "Used artifact power: " << power << "\n";
    }
    else
    {
        std::cout << "No artifact powers available.\n";
    }
}

bool Player::hasArtifactPowers() const
{
    return !artifactPowers.empty();
}

int Player::getArtifactCount() const
{
    return static_cast<int>(artifactPowers.size());
}

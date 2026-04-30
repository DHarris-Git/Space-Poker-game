#pragma once
#include <stack>
#include <string>

//   /\_/\
//  ( o.o )  Player state: string + char[] + stack + recursion
//   > ^ <
class Player
{
private:
    std::string name;
    char shipName[50];

    int totalScore;     // score for the currently played mission
    int multiplier;     // multiplier for overflow bonus
    int roundPoints;    // points earned in current round
    int targetScore;    // target for current mission

    std::stack<int> artifactPowers;

    // Recursive text reversal used for alien-display effects.
    std::string reverseRecursive(const std::string& text, int index) const;

public:
    Player(std::string playerName);

    void setShipName(const char* ship);
    std::string getName() const;
    std::string getDisplayName(bool alienReversal) const;
    std::string getDisplayShipName(bool alienReversal) const;

    int getTotalScore() const;

    void startRaid(int target);
    void addHandScore(int points);
    void commitRoundScore();

    bool raidSuccess() const;
    void applyOverflowBonus();
    void resetTotalScore();

    void addArtifactPower(int power);
    void useArtifactPower();
    bool hasArtifactPowers() const;
    int getArtifactCount() const;

    // Recursive overflow bonus calculation.
    int recursiveMultiplierBonus(int overflow);
};

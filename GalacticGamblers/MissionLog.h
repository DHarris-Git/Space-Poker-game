#pragma once
#include <deque>
#include <stack>
#include <string>
#include "Inventory.h"
#include "Player.h"

//   Mission flow:
//   FIFO raids via deque  <->  LIFO reversal via stack
struct Mission
{
    std::string title;
    std::string description;
    int targetScore;
    bool completed;
};

class MissionLog
{
public:
    std::deque<Mission> missions;   // queue-like mission order
    int activeMissionIndex;

private:
    std::stack<int> completedMissionStack; // reversal history

public:
    MissionLog();

    void addMission(const std::string& title, const std::string& desc, int score);
    bool hasMissions() const;

    int getPlayableMissionIndex(bool alienReversal) const;
    bool canPlayMission(int missionIndex, bool alienReversal) const;
    bool hasCompletedHistory() const;

    void showMissionLog(bool alienReversal) const;
    bool completeCurrentMission(Player& player, Inventory& inventory, bool alienReversal);

    void updateActiveMissionIndex();
};

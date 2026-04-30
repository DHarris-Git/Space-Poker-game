#include "MissionLog.h"
#include <iostream>

MissionLog::MissionLog()
{
    activeMissionIndex = 0;
}

void MissionLog::addMission(const std::string& title, const std::string& desc, int score)
{
    missions.push_back({ title, desc, score, false });
    updateActiveMissionIndex();
}

bool MissionLog::hasMissions() const
{
    for (const auto& m : missions)
    {
        if (!m.completed)
            return true;
    }
    return false;
}

int MissionLog::getPlayableMissionIndex(bool alienReversal) const
{
    if (missions.empty())
        return -1;

    // Alien mode replays missions from last completed -> first completed.
    if (alienReversal && !completedMissionStack.empty())
        return completedMissionStack.top();

    // Normal mode always targets the first incomplete mission.
    return activeMissionIndex;
}

bool MissionLog::canPlayMission(int missionIndex, bool alienReversal) const
{
    return missionIndex == getPlayableMissionIndex(alienReversal);
}

bool MissionLog::hasCompletedHistory() const
{
    return !completedMissionStack.empty();
}

void MissionLog::updateActiveMissionIndex()
{
    for (size_t i = 0; i < missions.size(); ++i)
    {
        if (!missions[i].completed)
        {
            activeMissionIndex = static_cast<int>(i);
            return;
        }
    }

    activeMissionIndex = 0;
}

void MissionLog::showMissionLog(bool alienReversal) const
{
    const int playable = getPlayableMissionIndex(alienReversal);

    std::cout << "----- Mission Log -----\n";
    for (size_t i = 0; i < missions.size(); ++i)
    {
        const Mission& m = missions[i];

        std::cout << (i + 1) << ". ";

        if (static_cast<int>(i) == playable)
        {
            if (alienReversal && !completedMissionStack.empty())
                std::cout << "[Reversal Target] ";
            else
                std::cout << "[Active] ";
        }

        std::cout << m.title << " | " << m.description;

        if (m.completed)
            std::cout << " [Completed]";

        std::cout << "\n";
    }
    std::cout << "----------------------\n";
}

bool MissionLog::completeCurrentMission(Player& player, Inventory& inventory, bool alienReversal)
{
    if (missions.empty())
        return false;

    const int missionIndex = getPlayableMissionIndex(alienReversal);
    if (missionIndex < 0 || missionIndex >= static_cast<int>(missions.size()))
        return false;

    Mission& m = missions[missionIndex];

    if (player.getTotalScore() < m.targetScore)
    {
        std::cout << "Mission incomplete! Not enough points.\n";
        return false;
    }

    if (alienReversal && !completedMissionStack.empty())
    {
        if (!player.hasArtifactPowers() || inventory.isEmpty())
        {
            std::cout << "Alien reversal active, but no stolen artifacts to return.\n";
            return false;
        }

        std::string returnedItem;
        if (!inventory.removeTopItem(returnedItem))
        {
            std::cout << "Alien reversal failed while returning inventory item.\n";
            return false;
        }

        // LIFO undo: return latest artifact and re-open the mission.
        player.useArtifactPower();
        m.completed = false;
        completedMissionStack.pop();

        std::cout << "Alien reversal! You returned " << returnedItem
            << " to " << m.description << ".\n";

        updateActiveMissionIndex();
        return true;
    }

    if (alienReversal && completedMissionStack.empty())
        std::cout << "Alien attack active, but nothing to return yet. Continuing FIFO raids.\n";

    if (m.completed)
    {
        std::cout << "Mission already completed.\n";
        return false;
    }

    // FIFO completion path: steal artifact and record mission in stack history.
    inventory.addItem(m.title, m.targetScore, 1);
    player.addArtifactPower(m.targetScore);

    m.completed = true;
    completedMissionStack.push(missionIndex);

    std::cout << "Mission completed! You stole " << m.title << ".\n";

    updateActiveMissionIndex();
    return true;
}

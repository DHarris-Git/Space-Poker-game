#include <algorithm>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef byte

#include "AlienAttack.h"
#include "CardGame.h"
#include "GameMap.h"
#include "Inventory.h"
#include "MissionLog.h"
#include "Player.h"


// Small hand bonus based on collected artifacts and hand tier.
int calculateArtifactBonus(HandType type, int artifactCount)
{
    if (artifactCount <= 0)
        return 0;

    int bonus = artifactCount;

    switch (type)
    {
    case PAIR:
    case TWO_PAIR:
        bonus += 1;
        break;
    case STRAIGHT:
    case FLUSH:
        bonus += artifactCount;
        break;
    case FULL_HOUSE:
    case FOUR_KIND:
    case STRAIGHT_FLUSH:
        bonus += artifactCount * 2;
        break;
    default:
        break;
    }

    return bonus;
}

int main()
{
    srand((unsigned)time(0));

    // --- SETUP ---
    std::cout << "Welcome to Galactic Gamblers!\n";
    std::cout << "Enter your name: ";
    std::string playerName;
    std::getline(std::cin, playerName);

    Player player(playerName);

    std::cout << "Choose your ship name (leave blank for The Golden Greg Ship): ";
    std::string shipName;
    std::getline(std::cin, shipName);
    if (shipName.empty())
        shipName = "The Golden Greg Ship";
    player.setShipName(shipName.c_str());

    AlienAttack alienAttack;
    Inventory inventory;
    MissionLog missions;
    GameMap galaxy;

    // --- CREATE SHIPS AND MISSIONS ---
    galaxy.addShip("Ship Alpha", 50, "Artifact Alpha");
    galaxy.addShip("Ship Beta", 40, "Artifact Beta");
    galaxy.addShip("Ship Gamma", 60, "Artifact Gamma");
    galaxy.addShip("Ship Delta", 30, "Artifact Delta");
    galaxy.addShip("Ship Epsilon", 55, "Artifact Epsilon");
    galaxy.addShip("Ship Zeta", 45, "Artifact Zeta");
    galaxy.addShip("Ship Omega", 70, "Artifact Omega");
    galaxy.addShip("Ship Theta", 25, "Artifact Theta");
    galaxy.addShip("Ship Iota", 65, "Artifact Iota");
    galaxy.addShip("Ship Kappa", 35, "Artifact Kappa");

    galaxy.connectShips("Ship Alpha", "Ship Beta");
    galaxy.connectShips("Ship Beta", "Ship Gamma");
    galaxy.connectShips("Ship Gamma", "Ship Delta");
    galaxy.connectShips("Ship Delta", "Ship Epsilon");
    galaxy.connectShips("Ship Epsilon", "Ship Zeta");
    galaxy.connectShips("Ship Zeta", "Ship Omega");
    galaxy.connectShips("Ship Alpha", "Ship Theta");
    galaxy.connectShips("Ship Theta", "Ship Iota");
    galaxy.connectShips("Ship Iota", "Ship Kappa");
    galaxy.connectShips("Ship Kappa", "Ship Omega");
    galaxy.connectShips("Ship Beta", "Ship Kappa");
    galaxy.connectShips("Ship Gamma", "Ship Iota");

    missions.addMission("Artifact Alpha", "Ship Alpha", 50);
    missions.addMission("Artifact Beta", "Ship Beta", 40);
    missions.addMission("Artifact Gamma", "Ship Gamma", 60);
    missions.addMission("Artifact Delta", "Ship Delta", 30);
    missions.addMission("Artifact Epsilon", "Ship Epsilon", 55);
    missions.addMission("Artifact Zeta", "Ship Zeta", 45);
    missions.addMission("Artifact Omega", "Ship Omega", 70);
    missions.addMission("Artifact Theta", "Ship Theta", 25);
    missions.addMission("Artifact Iota", "Ship Iota", 65);
    missions.addMission("Artifact Kappa", "Ship Kappa", 35);

    // --- PLAY INTRO ANIMATION ---
    playAsciiAnimation();

    // --- MAIN GAME LOOP ---
    while (missions.hasMissions())
    {
        alienAttack.tryTrigger();
        const bool alienReversal = alienAttack.isActive();

        if (alienReversal)
        {
            system("cls");
            std::cout << "========================================\n";
            std::cout << "!!! ALIEN ATTACK DETECTED !!!\n";
            std::cout << "Pilot: " << player.getDisplayName(true)
                << " | Ship: " << player.getDisplayShipName(true) << "\n";

            if (missions.hasCompletedHistory())
                std::cout << "Mission order is now LIFO. Return artifacts in reverse order.\n";
            else
                std::cout << "No stolen artifacts yet. Continue FIFO until inventory fills.\n";

            std::cout << "========================================\n";
            Sleep(1800);
        }

        bool missionSelected = false;
        Mission* currentMission = nullptr;

        while (!missionSelected)
        {
            system("cls");
            std::cout << "=== Mission Control ===\n";
            std::cout << "Player: " << player.getDisplayName(alienReversal)
                << " | Ship: " << player.getDisplayShipName(alienReversal) << "\n";
            std::cout << "Mode: " << (alienReversal ? "ALIEN REVERSAL (LIFO)" : "NORMAL (FIFO)") << "\n\n";

            missions.showMissionLog(alienReversal);
            std::cout << "\n[1-" << missions.missions.size()
                << "] Select | [S] Search Ships/Missions | [I] Inventory | [M] Galaxy Map\n";

            int key = _getch();

            // Handle mission input (1-9, and 0 for mission 10)
            if ((key >= '1' && key <= '9') || (key == '0' && missions.missions.size() >= 10))
            {
                const int missionNum = (key == '0') ? 9 : (key - '0') - 1;
                if (missionNum >= 0 && missionNum < (int)missions.missions.size())
                {
                    const Mission& m = missions.missions[missionNum];
                    const bool playable = missions.canPlayMission(missionNum, alienReversal);

                    std::string status;
                    if (playable)
                    {
                        if (alienReversal && missions.hasCompletedHistory())
                            status = "REVERSAL TARGET (Can Return)";
                        else
                            status = "ACTIVE (Can Play)";
                    }
                    else
                    {
                        status = m.completed ? "COMPLETED" : "LOCKED (Not Active)";
                    }

                    system("cls");
                    std::cout << "=== Mission Details ===\n";
                    std::cout << "Title: " << m.title << "\n";
                    std::cout << "Ship: " << m.description << "\n";
                    std::cout << "Target Score: " << m.targetScore << "\n";
                    std::cout << "Status: " << status << "\n\n";

                    if (playable)
                    {
                        if (alienReversal && missions.hasCompletedHistory())
                            std::cout << "Press ENTER to return this artifact, or any other key to go back.\n";
                        else
                            std::cout << "Press ENTER to raid this mission, or any other key to go back.\n";

                        const int choice = _getch();
                        if (choice == 13)
                        {
                            currentMission = &missions.missions[missionNum];
                            missionSelected = true;
                        }
                    }
                    else
                    {
                        std::cout << "You can only play the active target mission!\n";
                        std::cout << "Press any key to go back.\n";
                        _getch();
                    }
                }
            }
            // Search function with 'S' key
            else if (key == 's' || key == 'S')
            {
                system("cls");
                std::cout << "=== SEARCH ===\n";
                std::cout << "[1] Search Ships/Missions\n";
                std::cout << "[2] Search Inventory Items\n";
                std::cout << "Choice: ";
                int searchChoice = _getch();

                if (searchChoice == '1')
                {
                    std::cout << "\nEnter ship/mission name to search: ";
                    std::string keyword;
                    std::getline(std::cin, keyword);

                    bool found = false;
                    for (int i = 0; i < (int)missions.missions.size(); i++)
                    {
                        if (missions.missions[i].title.find(keyword) != std::string::npos ||
                            missions.missions[i].description.find(keyword) != std::string::npos)
                        {
                            system("cls");
                            const Mission& m = missions.missions[i];
                            std::cout << "=== FOUND ===\n";
                            std::cout << "Mission: " << m.title << "\n";
                            std::cout << "Ship: " << m.description << "\n";
                            std::cout << "Target Score: " << m.targetScore << " pts\n";
                            std::cout << "Status: " << (m.completed ? "COMPLETED" : "PENDING") << "\n";
                            std::cout << "\nPress any key to continue...\n";
                            _getch();
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        std::cout << "\nNo mission found.\n";
                        Sleep(1000);
                    }
                }
                else if (searchChoice == '2')
                {
                    std::cout << "\nEnter item name to search: ";
                    std::string itemKeyword;
                    std::getline(std::cin, itemKeyword);

                    const Item* current = inventory.findItem(itemKeyword);

                    if (current)
                    {
                        system("cls");
                        std::cout << "=== INVENTORY ITEM FOUND ===\n";
                        std::cout << "Item: " << current->name << "\n";
                        std::cout << "Value: " << current->value << " pts\n";
                        std::cout << "Rarity: " << current->rarity << "\n";
                        std::cout << "\nPress any key to continue...\n";
                        _getch();
                    }
                    else
                    {
                        std::cout << "\nItem not in inventory.\n";
                        Sleep(1000);
                    }
                }
            }
            // Inventory with 'I' key
            else if (key == 'i' || key == 'I')
            {
                system("cls");
                std::cout << "=== PLAYER INVENTORY ===\n";
                std::cout << "Player: " << player.getDisplayName(alienReversal)
                    << " | Ship: " << player.getDisplayShipName(alienReversal) << "\n";
                std::cout << "Mission Score: " << player.getTotalScore() << "\n\n";
                std::cout << "[1] LIFO Order  [2] Sort by Value  [3] Sort by Rarity\n";
                std::cout << "View: ";

                const int viewChoice = _getch();
                std::cout << "\n\n";

                if (viewChoice == '2')
                    inventory.printInventorySortedByValue();
                else if (viewChoice == '3')
                    inventory.printInventorySortedByRarity();
                else
                    inventory.printInventory();

                std::cout << "\nPress any key to go back...\n";
                _getch();
            }
            // Galaxy map with 'M' key
            else if (key == 'm' || key == 'M')
            {
                system("cls");
                std::cout << "=== Galaxy Map ===\n";
                galaxy.displayGalaxy(alienReversal);
                std::cout << "\nPress any key to return...\n";
                _getch();
            }
        }

        player.startRaid(currentMission->targetScore);

        // ---  CARD ANIMATION BEFORE POKER ---
        playAsciiAnimation();

        // --- POKER MINIGAME  ---
        int totalHandScore = 0;

        for (int handNumber = 1; handNumber <= 2; handNumber++)
        {
            createDeck();
            shuffleDeck();
            dealHand();

            bool handPlayed = false;
            while (!handPlayed)
            {
                drawCards();

                // Display mission info below the border
                std::cout << "\n========== MISSION INFO ==========\n";
                std::cout << "Mission: " << currentMission->title << " (" << currentMission->description << ")\n";
                std::cout << "Target: " << currentMission->targetScore << " pts | ";
                std::cout << "Current: " << (player.getTotalScore() + totalHandScore) << " pts | ";
                std::cout << "Artifacts: " << player.getArtifactCount() << "\n";
                std::cout << "HAND " << handNumber << "/2 | Mode: "
                    << (alienReversal ? "ALIEN REVERSAL" : "NORMAL") << "\n";
                std::cout << "==================================\n";

                int key = _getch();
                bool isExtended = false;
                if (key == 0 || key == 224)
                {
                    isExtended = true;
                    key = _getch();
                }

                // Handle arrow-key actions 
                if (isExtended)
                {
                    if (key == 75 && position > 0) position--;
                    if (key == 77 && position < 7) position++;
                    if (key == 72) selected[position] = !selected[position];
                    if (key == 80) discarded[position] = !discarded[position];
                }
                else
                {
                    if (key == 'd' || key == 'D') performDiscard();
                    if (key == 'h' || key == 'H') showHandGuide();
                    if (key == 13)
                    {
                        animatePlayHand();
                        handPlayed = true;
                    }
                }
            }

            std::vector<Card> playedCards;
            for (int i = 0; i < 8; i++)
            {
                if (selected[i])
                    playedCards.push_back(hand[i]);
            }

            HandType type = evaluateHand(playedCards);
            int baseHandScore = 0;
            switch (type)
            {
            case HIGH_CARD: baseHandScore = 10; break;
            case PAIR: baseHandScore = 20; break;
            case TWO_PAIR: baseHandScore = 30; break;
            case THREE_KIND: baseHandScore = 40; break;
            case STRAIGHT: baseHandScore = 50; break;
            case FLUSH: baseHandScore = 60; break;
            case FULL_HOUSE: baseHandScore = 70; break;
            case FOUR_KIND: baseHandScore = 80; break;
            case STRAIGHT_FLUSH: baseHandScore = 100; break;
            }

            const int artifactBonus = calculateArtifactBonus(type, player.getArtifactCount());
            const int finalHandScore = baseHandScore + artifactBonus;

            totalHandScore += finalHandScore;
            player.addHandScore(finalHandScore);

            std::cout << "\nHand " << handNumber << ": You played a " << handName(type)
                << " for " << baseHandScore << " points";
            if (artifactBonus > 0)
                std::cout << " + " << artifactBonus << " artifact bonus";
            std::cout << " = " << finalHandScore << " total!\n";

            std::cout << "Total Points So Far: " << totalHandScore << "\n";

            // Reset hand for next round
            hand.clear();
            std::fill(std::begin(selected), std::end(selected), false);
            std::fill(std::begin(discarded), std::end(discarded), false);
            discardsRemaining = 2;

            if (handNumber < 2)
            {
                std::cout << "\nPress any key for second hand...\n";
                _getch();
            }
        }

        // After both hands are played, commit score and check mission
        std::cout << "\nTotal hand score: " << totalHandScore << " points!\n";
        player.commitRoundScore();
        std::cout << "Mission Score is now: " << player.getTotalScore() << "\n";

        if (player.raidSuccess())
        {
            player.applyOverflowBonus();
            std::cout << "Round SUCCESS! Mission Score updated to: " << player.getTotalScore() << "\n";
        }
        else
        {
            std::cout << "Round incomplete! Did not reach target.\n";
        }
        // complete mission
        const bool missionResolved = missions.completeCurrentMission(player, inventory, alienReversal);
        if (missionResolved)
            player.resetTotalScore();

        std::cout << "\n=== Mission State Updated ===\n";
        std::cout << "Current mission score reset to: " << player.getTotalScore() << "\n\n";

        if (!inventory.isEmpty())
        {
            std::cout << "Inventory:\n";
            inventory.printInventory();
        }

        std::cout << "\nPress any key to continue...\n";
        _getch();
    }

    std::cout << "\nAll missions complete! Congratulations, " << player.getName() << "!\n";
    return 0;
}





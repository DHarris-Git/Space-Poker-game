// [GameMap] BST insertion/search + graph connectivity
#include "GameMap.h"
#include <iostream>

GameMap::GameMap()
{
    root = nullptr;
}

ShipNode* GameMap::insertShip(ShipNode* node, const std::string& name, int target, const std::string& reward)
{
    if (node == nullptr)
    {
        ShipNode* newShip = new ShipNode;

        newShip->name = name;
        newShip->targetScore = target;
        newShip->reward = reward;

        newShip->left = nullptr;
        newShip->right = nullptr;

        return newShip;
    }

    if (name < node->name)
        node->left = insertShip(node->left, name, target, reward);
    else
        node->right = insertShip(node->right, name, target, reward);

    return node;
}

void GameMap::addShip(const std::string& name, int targetScore, const std::string& reward)
{
    root = insertShip(root, name, targetScore, reward);
}

ShipNode* GameMap::searchShip(ShipNode* node, const std::string& name) const
{
    if (node == nullptr || node->name == name)
        return node;

    if (name < node->name)
        return searchShip(node->left, name);

    return searchShip(node->right, name);
}

ShipNode* GameMap::findShip(const std::string& name) const
{
    return searchShip(root, name);
}

void GameMap::connectShips(const std::string& a, const std::string& b)
{
    ShipNode* shipA = findShip(a);
    ShipNode* shipB = findShip(b);

    if (shipA && shipB)
    {
        // Undirected edge between ships in the galaxy graph.
        shipA->connections.push_back(shipB);
        shipB->connections.push_back(shipA);
    }
}

std::string GameMap::reverseRecursive(const std::string& text, int index) const
{
    if (index < 0)
        return "";

    return text[static_cast<size_t>(index)] + reverseRecursive(text, index - 1);
}

void GameMap::displayGalaxy(bool alienReversal) const
{
    printBST(root, alienReversal);
}

void GameMap::printBST(ShipNode* node, bool alienReversal) const
{
    if (node == nullptr)
        return;

    printBST(node->left, alienReversal);

    const std::string shipName = alienReversal
        ? reverseRecursive(node->name, static_cast<int>(node->name.size()) - 1)
        : node->name;

    const std::string rewardName = alienReversal
        ? reverseRecursive(node->reward, static_cast<int>(node->reward.size()) - 1)
        : node->reward;

    std::cout << "Ship: " << shipName << "\n";
    std::cout << "Raid Score Target: " << node->targetScore << "\n";
    std::cout << "Reward: " << rewardName << "\n";

    std::cout << "Connections: ";
    for (const ShipNode* ship : node->connections)
    {
        const std::string connectedName = alienReversal
            ? reverseRecursive(ship->name, static_cast<int>(ship->name.size()) - 1)
            : ship->name;

        std::cout << connectedName << " ";
    }

    std::cout << "\n\n";

    printBST(node->right, alienReversal);
}



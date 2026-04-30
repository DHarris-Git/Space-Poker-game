#pragma once
#include <string>
#include <vector>

//   ____  ____  ____
//  / __ \/ __ \/ __/
// / /_/ / /_/ /\ \   BST nodes + graph edges + search
// \____/\____/___/
struct ShipNode
{
    std::string name;
    int targetScore;
    std::string reward;

    // Graph links between ships.
    std::vector<ShipNode*> connections;

    // BST links for ordered ship lookup.
    ShipNode* left;
    ShipNode* right;
};

class GameMap
{
private:
    ShipNode* root;

    ShipNode* insertShip(ShipNode* node, const std::string& name, int target, const std::string& reward);
    ShipNode* searchShip(ShipNode* node, const std::string& name) const;
    void printBST(ShipNode* node, bool alienReversal) const;
    std::string reverseRecursive(const std::string& text, int index) const;

public:
    GameMap();

    void addShip(const std::string& name, int targetScore, const std::string& reward);
    ShipNode* findShip(const std::string& name) const;
    void connectShips(const std::string& a, const std::string& b);

    void displayGalaxy(bool alienReversal) const;
};

#pragma once
#include <string>

//  +------------------+
//  | Inventory (LL)   |
//  | + sort views     |
//  +------------------+
struct Item
{
    std::string name;
    int value;
    int rarity;
    Item* next;
};

class Inventory
{
private:
    Item* head;

public:
    Inventory();
    ~Inventory();

    void addItem(const std::string& name, int value, int rarity);
    bool removeTopItem(std::string& removedName);

    // Default linked-list (LIFO) view.
    void printInventory() const;

    // Sorted views use a sorting algorithm on copied pointers.
    void printInventorySortedByValue() const;
    void printInventorySortedByRarity() const;

    bool isEmpty() const;
    Item* findItem(const std::string& name) const;
};

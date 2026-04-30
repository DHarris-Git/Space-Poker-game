// [Inventory] linked-list storage + sorted views
#include "Inventory.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

namespace {
std::string toLower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

// Copy linked-list nodes into an array for sorted display.
std::vector<const Item*> collectItems(const Item* head)
{
    std::vector<const Item*> items;
    const Item* current = head;
    while (current)
    {
        items.push_back(current);
        current = current->next;
    }
    return items;
}

void printItems(const std::vector<const Item*>& items)
{
    if (items.empty())
    {
        std::cout << "Inventory is empty.\n";
        return;
    }

    std::cout << "----- Inventory -----\n";
    for (const Item* item : items)
    {
        std::cout << "- " << item->name
            << " (Value: " << item->value
            << ", Rarity: " << item->rarity << ")\n";
    }
    std::cout << "--------------------\n";
}
}

Inventory::Inventory()
{
    head = nullptr;
}

Inventory::~Inventory()
{
    Item* current = head;
    while (current)
    {
        Item* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

void Inventory::addItem(const std::string& name, int value, int rarity)
{
    Item* newItem = new Item{ name, value, rarity, head };
    head = newItem;
}

bool Inventory::removeTopItem(std::string& removedName)
{
    if (!head)
        return false;

    Item* top = head;
    removedName = top->name;
    head = top->next;
    delete top;
    return true;
}

void Inventory::printInventory() const
{
    printItems(collectItems(head));
}

void Inventory::printInventorySortedByValue() const
{
    std::vector<const Item*> items = collectItems(head);
    std::stable_sort(items.begin(), items.end(), [](const Item* a, const Item* b) {
        if (a->value == b->value)
            return a->rarity > b->rarity;
        return a->value > b->value;
    });

    printItems(items);
}

void Inventory::printInventorySortedByRarity() const
{
    std::vector<const Item*> items = collectItems(head);
    std::stable_sort(items.begin(), items.end(), [](const Item* a, const Item* b) {
        if (a->rarity == b->rarity)
            return a->value > b->value;
        return a->rarity > b->rarity;
    });

    printItems(items);
}

bool Inventory::isEmpty() const
{
    return head == nullptr;
}

Item* Inventory::findItem(const std::string& name) const
{
    const std::string query = toLower(name);

    Item* current = head;
    while (current)
    {
        const std::string itemName = toLower(current->name);
        if (itemName.find(query) != std::string::npos)
            return current;

        current = current->next;
    }

    return nullptr;
}



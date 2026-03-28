#pragma once
#include "../items/Item.hpp"
#include <vector>
#include <utility>
#include <memory>
#include "../core/SaveData.hpp"

class Inventory {
private:
    std::vector<std::pair<bool, std::shared_ptr<Item>>> inventory;
public:
    Inventory();
    Inventory(int inventorySize);
    ~Inventory();
    void addItem(std::shared_ptr<Item> item);
    std::shared_ptr<Item> getItem(int itemIndexInInventory);
};
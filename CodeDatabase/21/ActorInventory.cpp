#include "ActorInventory.hpp"




void Inventory::addItem(std::shared_ptr<Item> item) {
    (void)item;
}


std::shared_ptr<Item> Inventory::getItem(int itemIndexInInventory) {
    inventory[itemIndexInInventory].first = false;
    return inventory[itemIndexInInventory].second;
}


Inventory::Inventory() : Inventory(10) {}


Inventory::Inventory(int inventorySize) {
    for (int i = 0; i < inventorySize; ++i) {

    }
}


Inventory::~Inventory() {
}
#include "ShopItem.h"

ShopItem::ShopItem(Spell* spell, std::string description, std::string title, int cost) {
    this->description = description;
    this->title = title;
    this->cost = cost;
    this->spell = spell;
}

ShopItem::~ShopItem() {
    delete spell;
}
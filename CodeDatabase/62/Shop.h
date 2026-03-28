#pragma once

#include "ShopItem.h"
#include "Constants.h"
#include <vector>

class Shop {
private:
    std::vector<ShopItem*> items;
public:
    Shop();
    ~Shop() = default;
    std::vector<ShopItem*> getItems() const { return items; }

    size_t getSize() const { return items.size(); }

    const ShopItem* getItem(size_t index) const { return items[index]; }
    ShopItem* getItem(size_t index) { return items[index]; }

    void addItem(ShopItem* item) { items.push_back(item); }

    void removeItem(size_t index);
    void removeItem(const ShopItem* item);
};
#ifndef INCLUDE_SHOP
#define INCLUDE_SHOP

#include <vector>
#include "../items/spell.h"

class ShopItem {
private:
    Spell* spell_;
    int price_;

public:
    ShopItem(Spell* spell, int price);
    ~ShopItem();
    
    const Spell* spell() const { return spell_; }
    int price() const { return price_; }
    Spell* CreateSpell() const { return spell_->Clone(); }
};

class Shop {
private:
    std::vector<ShopItem*> items_;

public:
    Shop();
    ~Shop();
    
    void AddItem(Spell* spell, int price);
    int item_count() const { return items_.size(); }
    const ShopItem* get_item(int index) const;
    
    bool BuyItem(int index, class Player& player);
    void DisplayShop() const;
};

#endif
#include "../include/ui/shop.h"
#include "../include/entity/player.h"
#include <iostream>

ShopItem::ShopItem(Spell* spell, int price)
    : spell_(spell), price_(price) {}

ShopItem::~ShopItem() {
    delete spell_;
}

Shop::Shop() {
    AddItem(new HealSpell(30, 5), 5);
    AddItem(new DamageSpell(40, 3), 5);
    AddItem(new SplashSpell(80, 3, 2, 10), 10);
    AddItem(new TrapSpell(50, 7), 7);
    AddItem(new SummonSpell(1, 8), 8);
    AddItem(new EnhanceSpell(4), 4);
}

Shop::~Shop() {
    for (ShopItem* item : items_) {
        delete item;
    }
}

void Shop::AddItem(Spell* spell, int price) {
    items_.push_back(new ShopItem(spell, price));
}

const ShopItem* Shop::get_item(int index) const {
    return items_[index];
}

bool Shop::BuyItem(int index, Player &player) {
    if (index < 0 || index >= static_cast<int>(items_.size())) {
        return false;
    }
    
    const ShopItem* item = items_[index];
    
    if (player.score() < item->price()) {
        std::cout << "Недостаточно очков! Нужно: " << item->price() 
                  << ", У вас: " << player.score() << std::endl;
        return false;
    }
    
    if (player.inventory().is_full()) {
        std::cout << "Инвентарь полон!" << std::endl;
        return false;
    }
    
    player.spend_score(item->price());
    player.inventory().add_spell(item->CreateSpell());
    
    std::cout << "Куплено: " << item->spell()->name() << " за " << item->price() << " очков!" << std::endl;
    return true;
}

void Shop::DisplayShop() const {
    std::cout << "\n=== МАГАЗИН ЗАКЛИНАНИЙ ===" << std::endl;
    for (size_t i = 0; i < items_.size(); ++i) {
        const ShopItem* item = items_[i];
        std::cout << "[" << (i + 1) << "] " << item->spell()->name() 
                  << " - " << item->spell()->description()
                  << " (Цена: " << item->price() << " очков)" << std::endl;
    }
    std::cout << "[0] Выход из магазина" << std::endl;
}
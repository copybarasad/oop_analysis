#include "Shop.h"
#include "InstantDamageSpell.h"
#include "AllySpell.h"
#include "BoostSpell.h"
#include "HealthSpell.h"
#include "SplashDamageSpell.h"
#include "TrapSpell.h"

Shop::Shop(): items() {
    items.push_back(new ShopItem(new InstantDamageSpell(), "Apply instant\n10 damage in area", "Instant Damage Spell", 1));
    items.push_back(new ShopItem(new AllySpell(), "Spawn an ally\nwhich fights for you", "Ally", 3));
    items.push_back(new ShopItem(new BoostSpell(), "Boost your stats", "Boost Spell", 2));
    items.push_back(new ShopItem(new HealthSpell(), "Heal yourself", "Heal Spell", 1));
    items.push_back(new ShopItem(new SplashDamageSpell(2), "Apply\nsplashdamage in area", "Splash Damage", 1));
    items.push_back(new ShopItem(new TrapSpell(1, 40), "Apply trap\nin area", "Trap Spell", 1));
}

void Shop::removeItem(size_t index) {
    ShopItem* item = items[index];
    if(item) {
        delete item;
        items.erase(items.begin() + index);
    }
}

void Shop::removeItem(const ShopItem* item) {
    for(auto it = items.begin(); it != items.end(); ++it) {
        if(*it == item) {
            delete *it;
            items.erase(it);
            break;
        }
    }
}
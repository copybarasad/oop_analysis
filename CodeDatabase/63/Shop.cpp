#include "Shop.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Player.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"

Shop::Shop() {
    shop.push_back({"Прямой урон", [](){ return std::make_shared<DirectDamageSpell>(7,8); }, 150});
    shop.push_back({"Урон по площади", [](){ return std::make_shared<AreaDamageSpell>(7,10); }, 200});
    shop.push_back({"Мимик (ловушка)", [](){ return std::make_shared<TrapSpell>(7,10); }, 120});
    shop.push_back({"Призыв союзников", [](){ return std::make_shared<SummonSpell>(3,15); }, 180});
    shop.push_back({"Улучшение", [](){ return std::make_shared<UpgradeSpell>(10); }, 100});
    items.reserve(shop.size());
    for (int i = 0; i < shop.size(); i++)
        items.emplace_back("");
}

void Shop::buyItem(int index, Player& player) {
    int cost = shop[index].cost;
    if (player.getPoints() < cost) return;
    auto& spellsHand = player.getSpellsHand();
    if (spellsHand.getSpells().size() >= spellsHand.getMaxSize() || index >= shop.size()) return;
    player.addPoints(-cost);
    spellsHand.addSpell(shop[index].factory());
}

Item* Shop::getItem(int index) {
    if (index < shop.size())
        return &shop[index];
    return nullptr;
}

void Shop::bindItem(int index, char symbol) {
    if (index < shop.size())
        items[index] = std::string(1, symbol) + " - " + shop[index].name + " - цена: \033[33m" + std::to_string(shop[index].cost) + "\033[m";
}

std::string Shop::getInfo(Player& player) {
    std::string info;
    info += "-- Магазин --\n";
    info += "Ваш баланс: " + std::to_string(player.getPoints()) + "\n";
    info += "Используйте указанные клавиши для покупки или C для выхода.\n";
    for (auto& item : items)
        info += item + "\n";
    return info;
}

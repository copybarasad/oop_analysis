#ifndef RPG_SHOP_H
#define RPG_SHOP_H

#include <functional>
#include <vector>
class ISpell;
class Player;

struct Item {
    std::string name;
    std::function<std::shared_ptr<ISpell>()> factory;
    int cost;
};

class Shop {
    std::vector<Item> shop;
    std::vector<std::string> items;
public:
    Shop();

    Item* getItem(int index);

    void buyItem(int index, Player& player);

    void bindItem(int index, char symbol);

    std::string getInfo(Player& player);
};

#endif
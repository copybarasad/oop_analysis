#pragma once
#include "../core/SaveData.hpp"

struct GameContext;

class Item {
protected:
    int countOfItem;
public:
    Item() : Item(0) {};
    Item(int count) { countOfItem = count; };
    int getCountOfItem() { return countOfItem; };
    void setCountOfItem(int newCount) { countOfItem = newCount; };
    void decCountOfItem() { --countOfItem; };
    void incCountOfItem() { ++countOfItem; };
    virtual void useItem(GameContext& ctx, int userIndex, int power) = 0;
    virtual ~Item() = default;
    virtual SpellSaveData getSpellSaveData() = 0;
};
#pragma once
#include <memory>
#include <vector>
#include <random>
#include "../core/SaveData.hpp"

struct GameContext;
class Item;

class Hand {
private:
    std::shared_ptr<Item> itemInHand;
    std::vector<std::pair<bool, std::shared_ptr<Item>>> allItems;
    int powerUp;
    int maxSize;
    int currentSize;
public:
    Hand(int sizeOfHand);
    Hand();
    HandSaveData getHandSavedata();
    void setHandSavedata(HandSaveData data);
    void updateSize();
    bool useItem(GameContext &ctx, int userIndex, int powerUp);
    void swapItem(int indexInHand);
    std::vector<std::pair<bool, int>> getHandData();
    int getCurrentHandSize();
    int getMaxHandSize();
    int getPowerOfSpell();
    void setPowerOfSpell(int newPower);
    void incPowerOfSpell();
    void addSpells(int indexInHand, int count);
    std::pair<int, int> getHandItemDamageAndDistance();
    void deleteHalfSpells();
private:
    void setRandomSpell();
    void setAvailableSpells();
    void restoreActiveItem(int savedIndex);
    void setActiveItem(int index);
};
#include "ActorHand.hpp"
#include "../core/GameContext.hpp"
#include "../items/Item.hpp"
#include "../items/DirectDamageSpell.hpp"
#include "../items/AreaDamageSpell.hpp"
#include "../items/UpgradeSpell.hpp"
#include "../items/TrapSpell.hpp"


Hand::Hand() : Hand(50) {}


Hand::Hand(int sizeOfHand) {
    setAvailableSpells();
    itemInHand = nullptr;
    setRandomSpell();
    this->powerUp = 0;
    this->maxSize = sizeOfHand;
    this->currentSize = 1;
}


void Hand::updateSize() {
    int currentSize = 0;
    for (auto& item : allItems) {
        currentSize += item.second->getCountOfItem();
    }
    this->currentSize = currentSize;
}


bool Hand::useItem(GameContext &ctx, int userIndex, int power) {
    if (itemInHand->getCountOfItem()) {
        itemInHand->useItem(ctx, userIndex, power);
        if (power != 0) {
            powerUp = 0;
        }
        updateSize();
        return true;
    }
    return false;
}


void Hand::swapItem(int indexInHand) {
    if (allItems[indexInHand].second == itemInHand) {
        return;
    }
    if ((int)allItems.size() <= indexInHand) {
        return;
    }
    for (auto& item : allItems) {
        if (item.second == allItems[indexInHand].second) {
            item.first = false;
        }
        if (item.second == itemInHand) {
            item.first = true;
        }
    }
    itemInHand = allItems[indexInHand].second;
}


void Hand::setRandomSpell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 3);
    int randomNumber = distrib(gen);
    allItems[randomNumber].second->incCountOfItem();
    swapItem(randomNumber);
}


void Hand::setAvailableSpells() {
    std::pair<bool, std::shared_ptr<Item>> areaSpell{true, std::make_shared<AreaDamageSpell>()};
    std::pair<bool, std::shared_ptr<Item>> directSpell{true, std::make_shared<DirectDamageSpell>()};
    std::pair<bool, std::shared_ptr<Item>> updateSpell{true, std::make_shared<UpdateSpell>()};
    std::pair<bool, std::shared_ptr<Item>> trapSpell{true, std::make_shared<TrapSpell>()};
    allItems.push_back(areaSpell);
    allItems.push_back(directSpell);
    allItems.push_back(updateSpell);
    allItems.push_back(trapSpell);
}


std::vector<std::pair<bool, int>> Hand::getHandData() {
    std::vector<std::pair<bool, int>> data;
    for (auto& item : allItems) {
        std::pair<bool, int> currItem = {item.first, item.second->getCountOfItem()};
        data.push_back(currItem);
    }
    return data;
}


std::pair<int, int> Hand::getHandItemDamageAndDistance() {
    return {0,0};
}


int Hand::getCurrentHandSize() {
    return currentSize;
}


int Hand::getMaxHandSize() {
    return maxSize;
}


int Hand::getPowerOfSpell() {
    return powerUp;
}


void Hand::setPowerOfSpell(int newPower) {
    powerUp = newPower;
}


void Hand::incPowerOfSpell() {
    ++powerUp;
}


void Hand::addSpells(int indexInHand, int count) {
    if (count + currentSize > maxSize) {
        count = maxSize - currentSize;
    }
    for (int i = 0; i < count; ++i) {
        allItems[indexInHand].second->incCountOfItem();
    }
    updateSize();
}


HandSaveData Hand::getHandSavedata() {
    HandSaveData data;
    data.currentSize = this->currentSize;
    data.maxSize = this->maxSize;
    data.powerUp = this->powerUp;
    data.areaSpell = allItems[0].second->getSpellSaveData();
    data.directSpell = allItems[1].second->getSpellSaveData();
    data.updateSpell = allItems[2].second->getSpellSaveData();
    data.trapSpell = allItems[3].second->getSpellSaveData();
    for (int i = 0; i < 4; ++i) {
        if (allItems[i].second == itemInHand) {
            data.itemInHand = i;
        }
    }
    return data;
}


void Hand::setHandSavedata(HandSaveData data) {
    this->allItems.clear();
    this->itemInHand = nullptr;
    this->maxSize = data.maxSize;
    this->powerUp = data.powerUp;
    this->allItems = {
        {false, std::make_shared<AreaDamageSpell>(data.areaSpell)},
        {false, std::make_shared<DirectDamageSpell>(data.directSpell)},
        {false, std::make_shared<UpdateSpell>(data.updateSpell)},
        {false, std::make_shared<TrapSpell>(data.trapSpell)}
    };
    restoreActiveItem(data.itemInHand);
    updateSize();
}

void Hand::restoreActiveItem(int savedIndex) {
    if (savedIndex >= 0 && savedIndex < (int)allItems.size() &&
        allItems[savedIndex].second->getCountOfItem() > 0) {
        setActiveItem(savedIndex);
        return;
    }
    for (int i = 0; i < (int)allItems.size(); ++i) {
        if (allItems[i].second->getCountOfItem() > 0) {
            setActiveItem(i);
            return;
        }
    }
    if (!allItems.empty()) {
        setActiveItem(0);
    }
}

void Hand::setActiveItem(int index) {
    for (auto& item : allItems) {
        item.first = false;
    }
    if (index >= 0 && index < (int)allItems.size()) {
        allItems[index].first = true;
        itemInHand = allItems[index].second;
    }
}


void Hand::deleteHalfSpells() {
    for (auto& [inHand, spell] : allItems) {
        if (spell) {
            int countOfSpell = spell->getCountOfItem();
            spell->setCountOfItem(countOfSpell/2);
        }
    }
}
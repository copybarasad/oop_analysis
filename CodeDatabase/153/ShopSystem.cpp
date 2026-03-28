#include "Player.h"
#include "Spell.h"
#include "SpellHand.h"
#include "ShopSystem.h"
#include "GameConstants.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

ShopSystem::ShopSystem() {
    availableSpells.push_back(std::unique_ptr<Spell>(new DirectDamageSpell(
        "Ice Arrow",
        GameConstants::ICE_ARROW_COST,
        GameConstants::ICE_ARROW_RANGE,
        GameConstants::ICE_ARROW_DAMAGE
    )));

    availableSpells.push_back(std::unique_ptr<Spell>(new AreaDamageSpell(
        "Ice Storm",
        GameConstants::ICE_STORM_COST,
        GameConstants::ICE_STORM_RANGE,
        GameConstants::ICE_STORM_DAMAGE
    )));
}

std::unique_ptr<Spell> ShopSystem::createSpellCopy(Spell* baseSpell) const {
    return baseSpell->clone();
}

int ShopSystem::calculateCost(Spell* spell) const {
    return spell->getManaCost();
}

bool ShopSystem::buySpell(Player& player, int spellIndex) const {
    if (spellIndex < 0 || spellIndex >= availableSpells.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }

    Spell* baseSpell = availableSpells[spellIndex].get();
    int cost = calculateCost(baseSpell);

    if (player.getScore() < cost) {
        std::cout << "Not enough points! Need " << cost << ", but you have " << player.getScore() << std::endl;
        return false;
    }

    if (player.getSpellHand().isFull()) {
        std::cout << "Your hand is full! Cannot buy more spells." << std::endl;
        return false;
    }

    auto newSpell = createSpellCopy(baseSpell);
    if (newSpell) {
        player.addScore(-cost);
        player.getSpellHand().addSpell(std::move(newSpell));
        std::cout << "Bought " << baseSpell->getName() << " for " << cost << " points!" << std::endl;
        return true;
    }

    return false;
}

const std::vector<std::unique_ptr<Spell>>& ShopSystem::getAvailableSpells() const {
    return availableSpells;
}

bool ShopSystem::interactWithPlayer(Player& player, InputHandler& inputHandler) const {
    if (player.getSpellHand().isFull()) {
        std::cout << "Your hand is full! Cannot buy more spells." << std::endl;
        return false;
    }

    if (availableSpells.empty()) {
        std::cout << "No spells available in the shop!" << std::endl;
        return false;
    }

    std::cout << "=== SPELL SHOP ===" << std::endl;
    std::cout << "Your score: " << player.getScore() << " points" << std::endl;
    std::cout << "Free spell slots: "
        << (player.getSpellHand().getMaxSize() - player.getSpellHand().getSpellCount())
        << "/" << player.getSpellHand().getMaxSize() << std::endl;
    std::cout << "Available spells:" << std::endl;

    for (int i = 0; i < availableSpells.size(); i++) {
        int cost = calculateCost(availableSpells[i].get());
        std::cout << i + 1 << ". " << availableSpells[i]->getDescription()
            << " - Cost: " << cost << " points" << std::endl;
    }

    int choice = inputHandler.getShopChoice(availableSpells.size());

    if (choice == 0) {
        std::cout << "Shop interaction cancelled." << std::endl;
        return true;
    }

    return processPlayerChoice(player, choice - 1);
}

bool ShopSystem::processPlayerChoice(Player& player, int spellIndex) const {
    return buySpell(player, spellIndex);
}
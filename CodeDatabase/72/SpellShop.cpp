//
// Created by Mac on 25.10.2025.
//

#include "SpellShop.h"

#include <iostream>
#include <unistd.h>

#include "spells/AreaDamageSpell.h"
#include "spells/DirectDamageSpell.h"
#include "spells/TrapSpell.h"

SpellShop::SpellShop(const int &currentLevel) {
    offers = createOffers(currentLevel);
}


std::vector<SpellOffer> SpellShop::createOffers(const int &currentLevel) {
    std::vector<SpellOffer> offers;
    offers.push_back({"Direct damage", 10 * currentLevel, [currentLevel](){ return std::make_unique<DirectDamageSpell>(50 * currentLevel, 3, 1); }});
    offers.push_back({"Fireball", 20 * currentLevel, [currentLevel](){ return std::make_unique<AreaDamageSpell>(30 * currentLevel, 2); }});
    offers.push_back({"TrapSpell", 12 * currentLevel, [currentLevel](){ return std::make_unique<TrapSpell>(40 * currentLevel); }});
    return offers;
}


void SpellShop::showOffers() const {
    std::cout << "\n=== Spell Shop ===\n";
    for (size_t i = 0; i < offers.size(); ++i) {
        std::cout << i + 1 << ". " << offers[i].name
                  << "  (Cost: " << offers[i].cost << " points)\n";
    }
    std::cout << "0. Exit shop\n";
}

void SpellShop::buySpell(Player& player) {
    showOffers();

    int choice;
    std::cout << "\nEnter number to buy (or 0 to exit): ";

    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cout << "Invalid input. Please enter a number.\n";
        sleep(2);
        return;
    }

    if (choice <= 0 || choice > offers.size()) {
        std::cout << "Leaving the shop.\n";
        return;
    }

    auto& offer = offers[choice - 1];
    if (player.getScore() < offer.cost) {
        std::cout << "Not enough points!\n";
        return;
    }

    if (!player.getHand()->canAddSpell()) {
        std::cout << "Your hand is full!\n";
        return;
    }

    player.addPoints(-offer.cost);
    player.getHand()->addSpell(offer.createSpell());
    std::cout << "You bought " << offer.name << "!\n";
}
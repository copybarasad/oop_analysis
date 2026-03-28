//
// Created by Mac on 25.10.2025.
//

#include "SpellController.h"

#include <iostream>
#include <unistd.h>

#include "../spells/DirectDamageSpell.h"

#define CLEAR_COMMAND "clear"

SpellController::SpellController(std::shared_ptr<Field> field)
    :field(field){}

void SpellController::useSpell() {

    system(CLEAR_COMMAND);
    auto hand = field->getPlayer()->getHand();

    if (hand->getCount() == 0) {
        std::cout << "Your hand is empty!\n";
        sleep(2);
        return;
    }

    std::cout << "\n=== Your Spells ===\n";
    hand->showHand();

    std::cout << "\nSelect spell index to cast (1-" << hand->getCount() << "), or 0 to cancel: ";
    int choice;

    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cout << "Invalid input. Please enter a number.\n";
        return;
    }

    if (choice <= 0 || choice > hand->getCount()) {
        std::cout << "Cancelled.\n";
        sleep(2);
        return;
    }

    auto spell = hand->getSpell(choice - 1);
    if (spell) {
        spell->cast(*field);
    } else {
        std::cout << "This spell cannot be cast right now!\n";
        sleep(2);
    }

}

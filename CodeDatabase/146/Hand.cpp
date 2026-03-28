#include "Hand.h"
#include "DirectSpell.h"
#include "TrapSpell.h"
#include <iostream>
#include "SquareSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"


SpellCard* Hand::create_spell_by_index(int index) {
    switch (index) {
    case 0: return new TrapSpell(15, 3);
    case 1: return new DirectSpell(8, 2);
    case 2: return new SquareSpell(100);
    case 3: return new SummonSpell();
    case 4: return new UpgradeSpell();
    default: return new DirectSpell(8, 2);
    }
}

void Hand::add_random_spell() {
    if (hand.size() >= hand_size) return;

    std::vector<std::string> spell_types = { "Trap Spell", "Direct Spell", "Square Spell", "Summon Spell", "Upgrade Spell" };
    SpellCard* new_spell = nullptr;

    if (hand.empty()) {
        int type = rand() % spell_types.size();
        new_spell = create_spell_by_index(type);
        hand.push_back(new_spell);
        std::cout << "First spell " << spell_types[type] << "\n";
        return;
    }

    std::cout << "Choose spell\n";
    for (int i = 0; i < spell_types.size(); ++i)
        std::cout << i << ": " << spell_types[i] << "\n";
    int choice = 0;
    std::cin >> choice;

    bool already_in_hand = false;
    SimpleTextFormatter formatter;
    for (auto* spell : hand) {
        if (spell->accept(formatter) == spell_types[choice]) {
            already_in_hand = true;
            break;
        }
    }
    if (already_in_hand) {
        std::cout << "Already have\n";
        add_random_spell();
        return;
    }

    new_spell = create_spell_by_index(choice);
    hand.push_back(new_spell);
    std::cout << "Spell " << spell_types[choice] << " added\n";
}


void Hand::plus_kill()
{
    kill_counter++;
    if (kill_counter >= count_kill_for_spell) {
        add_random_spell();
        kill_counter = 0;
    }
}

int Hand::consume_upgrades()
{
    int res = upgrade_count;
    upgrade_count = 0;
    return res;
}

void Hand::clear_half_hand()
{
    size_t n = hand.size();
    if (n <= 1) return;

    int to_remove = n / 2;

    for (size_t i = 0; i < to_remove; ++i)
    {
        int idx = rand() % hand.size();
        delete hand[idx];
        hand.erase(hand.begin() + idx);
    }
}

std::string Hand::print() {
    SimpleTextFormatter formatter;
    std::string result;

    for (size_t i = 0; i < hand.size(); ++i) {
        result += "[" + std::to_string(i) + "] " +
            hand[i]->accept(formatter);
        if (i < hand.size() - 1) {
            result += " | ";
        }
    }
    return result;
}

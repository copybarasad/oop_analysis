#include "has_hand.hpp"

HasHand::HasHand(int capacity): hand(capacity) {};

HasHand::HasHand(HandImage hand_image): hand(hand_image) {};

HasHand::HasHand(int capacity, std::shared_ptr<ISpell> initial_spell): 
    hand(Hand(capacity, initial_spell))
{};

std::shared_ptr<ISpell> HasHand::get_spell(int index) {
    return hand.get_spell(index);
}

bool HasHand::add_spell(std::shared_ptr<ISpell> spell) {
    return hand.add_spell(spell);
}
#include "BoostSpell.h"
#include "PlayerHand.h"
#include <iostream>

BoostSpell::BoostSpell(int boost_value)
    : boost_value_(boost_value), name_("Boost") {}

const std::string& BoostSpell::Name() const { return name_; }

bool BoostSpell::Cast(Game& /*game*/, PlayerHand& hand) {
    hand.AddBoostAll(boost_value_);
    return true;
}

std::unique_ptr<Spell> BoostSpell::Clone() const {
    return std::unique_ptr<Spell>(new BoostSpell(boost_value_));
}

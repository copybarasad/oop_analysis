#include "SummonSpell.h"
#include "Game.h"
#include "PlayerHand.h"
#include <iostream>

SummonSpell::SummonSpell(int base_count, int ally_hp, int ally_damage)
    : base_count_(base_count), ally_hp_(ally_hp), ally_damage_(ally_damage),
      name_("Summon Ally") {}

const std::string& SummonSpell::Name() const { return name_; }

bool SummonSpell::Cast(Game& game, PlayerHand& hand) {
    int extra = hand.ConsumeBoostForSummonCount();
    int count = base_count_ + extra;
    if (count < 1) count = 1;

    bool ok = game.SummonAlliesAroundPlayer(count, ally_hp_, ally_damage_);
    if (!ok) {
        std::cout << "No space to summon.\n";
    }
    return true;
}

std::unique_ptr<Spell> SummonSpell::Clone() const {
    return std::unique_ptr<Spell>(new SummonSpell(base_count_, ally_hp_, ally_damage_));
}

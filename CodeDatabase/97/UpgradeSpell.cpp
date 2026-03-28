#include "UpgradeSpell.h"
#include "Hand.h"
#include <sstream>

UpgradeSpell::UpgradeSpell(int addRange, int addWidth, int addHeight, int addTrap, int addSummon,
                           Field& field, Coord casterPos, std::optional<Coord> target,
                           std::shared_ptr<Player> caster, Hand* hand)
        : addRange_(addRange), addW_(addWidth), addH_(addHeight), addTrap_(addTrap), addSummon_(addSummon),
          field_(&field), casterPos_(casterPos), target_(target), caster_(caster), hand_(hand) {}

std::shared_ptr<Spell> UpgradeSpell::clone() const {
    return std::make_shared<UpgradeSpell>(*this);
}

std::string UpgradeSpell::name() const { return "Upgrade"; }
std::string UpgradeSpell::description() const {
    std::ostringstream ss;
    ss << "Upgrade: +R=" << addRange_ << " +W=" << addW_ << " +H=" << addH_ << " +TrapDmg=" << addTrap_ << " +Summon=" << addSummon_;
    return ss.str();
}

bool UpgradeSpell::apply() {
    if (!hand_) return false;
    UpgradeData data;
    data.addRange = addRange_;
    data.addW = addW_;
    data.addH = addH_;
    data.addTrap = addTrap_;
    data.addSummon = addSummon_;

    hand_->accumulateUpgrade(data);
    return true;
}

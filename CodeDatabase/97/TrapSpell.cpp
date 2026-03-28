#include "TrapSpell.h"
#include "Field.h"
#include "Trap.h"
#include <sstream>
#include <cmath>

TrapSpell::TrapSpell(int dmg, int range,
                     Field& field, Coord casterPos,
                     std::optional<Coord> target,
                     std::shared_ptr<Player> caster)
        : dmg_(dmg), range_(range), field_(&field), casterPos_(casterPos), target_(target), caster_(caster) {}

std::shared_ptr<Spell> TrapSpell::clone() const {
    return std::make_shared<TrapSpell>(*this);
}

std::string TrapSpell::name() const { return "Trap"; }
std::string TrapSpell::description() const {
    std::ostringstream ss; ss << "Places a trap that deals " << dmg_ << " when an enemy steps on it (range " << range_ << ")";
    return ss.str();
}

static int manhTrap(const Coord& a, const Coord& b) { return std::abs(a.row - b.row) + std::abs(a.col - b.col); }

bool TrapSpell::apply() {
    if (!target_) return false;
    Coord t = *target_;
    if (!field_->inBounds(t)) return false;
    if (manhTrap(casterPos_, t) > range_) return false;
    if (field_->cellAt(t).hasEntity()) return false;

    auto trap = std::make_shared<Trap>(dmg_);
    field_->placeEntity(trap, t);
    return true;
}




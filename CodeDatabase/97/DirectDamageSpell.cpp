#include "DirectDamageSpell.h"
#include "Field.h"
#include "Enemy.h"
#include "Player.h"
#include <cmath>
#include <sstream>

DirectDamageSpell::DirectDamageSpell(int dmg, int range, Field& field,
                                     Coord casterPos, std::optional<Coord> target,
                                     std::shared_ptr<Player> caster)
        : dmg_(dmg), range_(range), field_(&field), casterPos_(casterPos), target_(target), caster_(caster) {}

std::shared_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_shared<DirectDamageSpell>(*this);
}

std::string DirectDamageSpell::name() const { return "Direct Damage"; }
std::string DirectDamageSpell::description() const {
    std::ostringstream ss; ss << "Deals " << dmg_ << " to a single enemy/building within range " << range_;
    return ss.str();
}

static int manhattanDistDirect(const Coord& a, const Coord& b) {
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}

bool DirectDamageSpell::apply() {
    if (!target_) return false;
    Coord t = *target_;
    if (!field_->inBounds(t)) return false;

    if (manhattanDistDirect(casterPos_, t) > range_) return false;

    auto cell = field_->cellAt(t);
    auto ent = cell.getEntity();
    if (!ent) return false;

    ent->takeDamage(dmg_);
    if (!ent->isAlive()) {
        bool wasEnemy = (dynamic_cast<Enemy*>(ent.get()) != nullptr);
        field_->cellAt(t).clearEntity();
        if (wasEnemy && caster_) caster_->addScore(1);
    }
    return true;
}



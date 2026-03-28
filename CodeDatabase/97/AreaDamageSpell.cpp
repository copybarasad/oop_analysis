#include "AreaDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include <sstream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int dmg, int range, int w, int h,
                                 Field& field, Coord casterPos,
                                 std::optional<Coord> target,
                                 std::shared_ptr<Player> caster)
        : dmg_(dmg), range_(range), w_(w), h_(h), field_(&field), casterPos_(casterPos), target_(target), caster_(caster) {}

std::shared_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_shared<AreaDamageSpell>(*this);
}

std::string AreaDamageSpell::name() const { return "Area Damage"; }
std::string AreaDamageSpell::description() const {
    std::ostringstream ss; ss << "Deals " << dmg_ << " to a " << w_ << "x" << h_ << " area (range " << range_ << ")";
    return ss.str();
}

static int manhattanDistArea(const Coord& a, const Coord& b) {
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}

bool AreaDamageSpell::apply() {
    if (!target_) return false;
    Coord t = *target_;
    if (!field_->inBounds(t)) return false;

    if (manhattanDistArea(casterPos_, t) > range_) return false;

    for (int dr = 0; dr < h_; ++dr) {
        for (int dc = 0; dc < w_; ++dc) {
            Coord cur{t.row + dr, t.col + dc};
            if (!field_->inBounds(cur)) continue;
            auto ent = field_->cellAt(cur).getEntity();
            if (ent) {
                ent->takeDamage(dmg_);
                if (!ent->isAlive()) {
                    bool wasEnemy = (dynamic_cast<Enemy*>(ent.get()) != nullptr);
                    field_->cellAt(cur).clearEntity();
                    if (wasEnemy && caster_) caster_->addScore(1);
                }
            }
        }
    }

    return true;
}




#include "SummonSpell.h"
#include "Field.h"
#include "Ally.h"

#include <algorithm>
#include <sstream>
#include <random>
#include <ctime>

SummonSpell::SummonSpell(int count, int range,
                         Field& field, Coord casterPos,
                         std::optional<Coord> target,
                         std::shared_ptr<Player> caster)
        : count_(count), range_(range), field_(&field), casterPos_(casterPos), target_(target), caster_(caster) {}

std::shared_ptr<Spell> SummonSpell::clone() const {
    return std::make_shared<SummonSpell>(*this);
}

std::string SummonSpell::name() const { return "Summon"; }
std::string SummonSpell::description() const {
    std::ostringstream ss; ss << "Summons " << count_ << " ally(ies) near the caster (range " << range_ << ")";
    return ss.str();
}

bool SummonSpell::apply() {
    std::vector<Coord> candidates;
    for (int dr = -range_; dr <= range_; ++dr) for (int dc = -range_; dc <= range_; ++dc) {
            if (dr == 0 && dc == 0) continue;
            Coord c{casterPos_.row + dr, casterPos_.col + dc};
            if (!field_->inBounds(c)) continue;
            if (!field_->cellAt(c).hasEntity() && field_->cellAt(c).isPassable()) candidates.push_back(c);
        }
    if (candidates.empty()) return false;
    std::mt19937 gen((unsigned)time(nullptr));
    std::shuffle(candidates.begin(), candidates.end(), gen);
    int put = 0;
    for (auto &pos : candidates) {
        if (put >= count_) break;
        field_->placeEntity(std::make_shared<Ally>(3, 1), pos);
        ++put;
    }
    return (put > 0);
}


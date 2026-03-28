#include "PlayerHand.h"
#include <utility>

#include <random>

#include "DirectDamageSpell.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"
#include "GameExceptions.h"

PlayerHand::PlayerHand(size_t max_size) : max_size_(max_size) {}

bool PlayerHand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells_.size() < max_size_) {
        spells_.push_back(std::move(spell));
        return true;
    }
    return false;
}

void PlayerHand::useSpell(int index, Game& game, int target_x, int target_y) {
    if (index >= 0 && index < spells_.size()) {
        spells_[index]->apply(game, target_x, target_y);
        spells_.erase(spells_.begin() + index);
    }
}

const std::vector<std::unique_ptr<ISpell>>& PlayerHand::getSpells() const {
    return spells_;
}

bool PlayerHand::isFull() const {
    return spells_.size() >= max_size_;
}

void PlayerHand::save(std::ostream& os) const {
    os << spells_.size() << "\n";
    for (const auto& spell : spells_) {
        os << spell->getTypeId() << "\n";
    }
}

void PlayerHand::load(std::istream& is) {
    size_t count;
    if (!(is >> count)) return;

    spells_.clear();
    for (size_t i = 0; i < count; ++i) {
        int type;
        is >> type;
        if (is.fail()) throw CorruptedDataException("Failed to read spell type");

        switch (type) {
            case 0: spells_.push_back(std::make_unique<DirectDamageSpell>(25, 5)); break;
            case 1: spells_.push_back(std::make_unique<AoESpell>(15, 4, 1)); break;
            case 2: spells_.push_back(std::make_unique<TrapSpell>(40)); break;
            case 3: spells_.push_back(std::make_unique<SummonSpell>(1, 50, 15)); break;
            case 4: spells_.push_back(std::make_unique<BoostSpell>()); break;
            default: throw CorruptedDataException("Unknown spell type in save file: " + std::to_string(type));
        }
    }
}

void PlayerHand::removeRandomSpell() {
    if (spells_.empty()) return;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, spells_.size() - 1);

    int index_to_remove = dist(rng);
    spells_.erase(spells_.begin() + index_to_remove);
}
#include "player_hand.h"
#include "direct_damage_spell.h"
#include "aoe_spell.h"
#include "trap_spell.h"
#include <random>
#include "save_manager.h"

PlayerHand::PlayerHand(size_t maxSize) : maxSize_(maxSize) {}

void PlayerHand::addRandomSpell() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    SpellPtr spell;
    switch (dis(gen)) {
        case 0: spell = std::make_unique<DirectDamageSpell>(); break;
        case 1: spell = std::make_unique<AoESpell>(); break;
        case 2: spell = std::make_unique<TrapSpell>(); break;
    }
    addSpell(std::move(spell));
}

bool PlayerHand::addSpell(SpellPtr spell) {
    if (spells_.size() >= maxSize_) return false;
    spells_.push_back(std::move(spell));
    return true;
}

bool PlayerHand::useSpell(size_t index, GameController& game, int x, int y) {
    if (index >= spells_.size()) return false;
    bool success = spells_[index]->use(game, x, y);
    if (success) {
        spells_.erase(spells_.begin() + index);
        addRandomSpell();
    }
    return success;
}

bool PlayerHand::removeSpell(size_t index) {
    if (index >= spells_.size()) return false;
    spells_.erase(spells_.begin() + index);
    return true;
}

void PlayerHand::upgradeMaxSize(int delta) {
    if (maxSize_ < 5) {
        maxSize_ += delta;
        if (maxSize_ > 5) maxSize_ = 5;
    }
}

size_t PlayerHand::size() const { return spells_.size(); }
size_t PlayerHand::getMaxSize() const { return maxSize_; }
const std::vector<SpellPtr>& PlayerHand::getSpells() const { return spells_; }

void PlayerHand::save(std::ostream& os) const {
    os << spells_.size() << "\n";
    for (const auto& spell : spells_) {
        os << spell->getName() << "\n";
        spell->save(os);
    }
}

void PlayerHand::load(std::istream& is) {
    size_t size;
    is >> size;
    is.ignore();
    if (is.fail()) throw SaveDataException("Failed to read spell count");

    spells_.clear();
    for (size_t i = 0; i < size; ++i) {
        std::string name;
        std::getline(is, name);
        if (name.empty()) std::getline(is, name);
        if (name.empty()) throw SaveDataException("Empty spell name");

        SpellPtr spell;
        if (name == "Firebolt") spell = std::make_unique<DirectDamageSpell>();
        else if (name == "Explosion") spell = std::make_unique<AoESpell>();
        else if (name == "Spike Trap") spell = std::make_unique<TrapSpell>();
        else throw SaveDataException("Unknown spell: " + name);

        spell->load(is);
        spells_.push_back(std::move(spell));
    }
}
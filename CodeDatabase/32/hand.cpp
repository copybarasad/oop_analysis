#include "hand.hpp"
#include "game.hpp"
#include "direct_damage_spell.hpp"
#include "area_damage_spell.hpp"
#include "trap.hpp"
#include "summon_spell.hpp"
#include "enhancementspell.hpp"
#include "constants.hpp"

Hand::Hand(size_t capacity)
    : capacity_(capacity) {}

bool Hand::AddSpell(std::unique_ptr<Spell> spell) {
    return AddSpellWithLevel(std::move(spell), 0);
}

bool Hand::AddSpellWithLevel(std::unique_ptr<Spell> spell, int level) {
    if (spells_.size() >= capacity_) {
        return false;
    }
    spells_.push_back(std::move(spell));
    upgrade_levels_.push_back(level);
    return true;
}

bool Hand::UseSpell(size_t index, Game& game) {
    if (index >= spells_.size()) {
        std::cout << "❌ Неверный номер заклинания.\n";
        return false;
    }

    const std::string spellName = spells_[index]->GetName();
    // Заклинание тратится ВСЕГДА, даже если не попало
    bool success = spells_[index]->UseSpell(game);

    std::cout << "🔮 Заклинание " << spellName << " исчезает из руки!\n";

    spells_.erase(spells_.begin() + index);
    upgrade_levels_.erase(upgrade_levels_.begin() + index);

    return success;

    return success;
}

void Hand::PrintHand() const {
    if (spells_.empty()) {
        std::cout << "🃏 Рука пуста.\n";
        return;
    }

    std::cout << "💫 Заклинания в руке:\n";
    for (size_t i = 0; i < spells_.size(); ++i) {
        std::cout << (i + 1) << ") " << spells_[i]->GetName();
        if (upgrade_levels_[i] > 0) {
            std::cout << " (уровень " << upgrade_levels_[i] << ")";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Hand::RemoveRandomHalf() {
    if (spells_.empty()) return;

    size_t half = (spells_.size() + 1) / 2;

    for (size_t i = 0; i < half; ++i) {
        size_t index = std::rand() % spells_.size();
        spells_.erase(spells_.begin() + index);
        upgrade_levels_.erase(upgrade_levels_.begin() + index);
    }
}

void Hand::UpgradeSpell(size_t index) {
    if (index >= spells_.size()) {
        std::cout << "❌ Неверный индекс заклинания!\n";
        return;
    }

    Spell* spell = spells_[index].get();

    std::cout << "\n✨ Улучшаем заклинание: " << spell->GetName() << "\n";

    if (auto* d = dynamic_cast<DirectDamageSpell*>(spell)) {
        d->Upgrade();
    } else if (auto* a = dynamic_cast<AreaDamageSpell*>(spell)) {
        a->Upgrade();
    } else if (auto* t = dynamic_cast<TrapSpell*>(spell)) {
        t->Upgrade();
    } else if (auto* s = dynamic_cast<SummonSpell*>(spell)) {
        s->Upgrade();
    } else if (auto* e = dynamic_cast<EnhancementSpell*>(spell)) {
        e->Upgrade();
    } else {
        std::cout << "❌ Это заклинание нельзя улучшить.\n";
        return;
    }

    upgrade_levels_[index] += 1;
}

void Hand::Clear() {
    spells_.clear();
    upgrade_levels_.clear();
}

Spell* Hand::GetSpell(size_t index) {
    if (index >= spells_.size()) return nullptr;
    return spells_[index].get();
}

const Spell* Hand::GetSpell(size_t index) const {
    if (index >= spells_.size()) return nullptr;
    return spells_[index].get();
}

int Hand::GetUpgradeLevel(size_t index) const {
    if (index >= upgrade_levels_.size()) return 0;
    return upgrade_levels_[index];
}
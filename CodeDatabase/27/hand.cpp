#include "hand.h"

#include <algorithm>
#include <random>

#include "aoe_attack.h"
#include "direct_attack.h"
#include "trap.h"

Hand::Hand(size_t capacity) : capacity(capacity) {}

std::unique_ptr<ISpell> Hand::make(SpellId id) {
    switch (id) {
    case SpellId::DirectAttack:
        return std::make_unique<DirectAttack>("Derezz Bolt", 6, 20);
    case SpellId::AOEAttack:
        return std::make_unique<AOEAttack>("Grid Shock", 5, 12);
    case SpellId::Trap:
        return std::make_unique<Trap>("Data Snare", 3, 18);
    default:
        return nullptr;
    }
}

std::unique_ptr<ISpell> Hand::CreateSpellForId(SpellId id) {
    return make(id);
}

bool Hand::has_spell(SpellId id) const {
    for (const auto& s : spells) {
        if (s && s->id() == id) {
            return true;
        }
    }
    return false;
}

bool Hand::add_spell(std::unique_ptr<ISpell> spell) {
    if (!spell || spells.size() >= capacity) {
        return false;
    }
    if (has_spell(spell->id())) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool Hand::add_random_spell() {
    if (spells.size() >= capacity) {
        return false;
    }

    std::vector<SpellId> pool;
    for (SpellId id : {SpellId::DirectAttack, SpellId::AOEAttack, SpellId::Trap}) {
        if (!has_spell(id)) {
            pool.push_back(id);
        }
    }
    if (pool.empty()) {
        return false;
    }

    std::shuffle(pool.begin(), pool.end(), std::mt19937{std::random_device{}()});
    return add_spell(make(pool.back()));
}

void Hand::remove_random_half() {
    if (spells.size() <= 1) {
        return;
    }

    const size_t toRemove = spells.size() / 2;
    std::shuffle(spells.begin(), spells.end(), std::mt19937{std::random_device{}()});
    spells.erase(spells.begin(), spells.begin() + static_cast<std::ptrdiff_t>(toRemove));
}

std::vector<std::string> Hand::ListSpells() const {
    std::vector<std::string> lines;
    lines.reserve(spells.size());
    for (size_t i = 0; i < spells.size(); ++i) {
        const auto& sp = spells[i];
        if (!sp) {
            continue;
        }
        lines.emplace_back(std::to_string(i) + ") " + sp->name() + " | radius: " + std::to_string(sp->radius()));
    }
    return lines;
}

bool Hand::buy_spell(int score) {
    const int cost = 10;
    if (score < cost) {
        return false;
    }
    return add_random_spell();
}

bool Hand::cast_spell_by_index(size_t index, Player& player, Grid& grid, int tx, int ty) {
    if (index >= spells.size() || !spells[index]) {
        return false;
    }
    return spells[index]->cast(player, grid, tx, ty);
}

#include "player_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"

#include <fstream>
#include <cstdint>


PlayerHand::PlayerHand(int maxSize)
    : maxHandSize(maxSize) {
}

bool PlayerHand::addSpell(std::unique_ptr<SpellCard> spell) {
    if (isFull()) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

bool PlayerHand::canAddSpell() const {
    return !isFull();
}

bool PlayerHand::isEmpty() const {
    return spells.empty();
}

bool PlayerHand::isFull() const {
    return spells.size() >= maxHandSize;
}

int PlayerHand::getSize() const {
    return spells.size();
}

int PlayerHand::getMaxSize() const {
    return maxHandSize;
}

const std::vector<std::unique_ptr<SpellCard>>& PlayerHand::getSpells() const {
    return spells;
}

SpellCard* PlayerHand::getSpell(int index) const {
    if (index < 0 || index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

void PlayerHand::clear() {
    spells.clear();
}

void PlayerHand::save(std::ostream& out) const {
    uint32_t size = static_cast<uint32_t>(spells.size());
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(&maxHandSize), sizeof(maxHandSize));

    for (const auto& spell : spells) {
        if (!spell) continue;
        // Сохраняем тип заклинания
        uint8_t type = 0;
        if (dynamic_cast<DirectDamageSpell*>(spell.get())) type = 1;
        else if (dynamic_cast<AreaDamageSpell*>(spell.get())) type = 2;
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));

        // Сохраняем само заклинание
        spell->save(out);
    }
}

void PlayerHand::load(std::istream& in) {
    uint32_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    in.read(reinterpret_cast<char*>(&maxHandSize), sizeof(maxHandSize));

    spells.clear();
    for (uint32_t i = 0; i < size; ++i) {
        uint8_t type;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        std::unique_ptr<SpellCard> spell;
        if (type == 1) {
            spell = std::make_unique<DirectDamageSpell>("", "", 0, 0, nullptr);
        }
        else if (type == 2) {
            spell = std::make_unique<AreaDamageSpell>("", "", 0, 0, 0, nullptr);
        }
        if (spell) {
            spell->load(in);
            spells.push_back(std::move(spell));
        }
    }
}
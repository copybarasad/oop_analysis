#pragma once
#include <memory>
#include <vector>

#include "BasicSpell.h"


class PlayerHand {
private:
    std::vector<std::unique_ptr<BasicSpell>> spells;
    int size;
public:
    PlayerHand(const int size) : size(size) {}
    PlayerHand(const int size, std::vector<std::unique_ptr<BasicSpell>> spells) : spells(std::move(spells)), size(size) {}
    PlayerHand(PlayerHand&&) = default;
    PlayerHand& operator=(PlayerHand&&) = default;
    ~PlayerHand() = default;
    void addSpell(std::unique_ptr<BasicSpell> spell) { spells.push_back(std::move(spell)); }
    bool removeSpell(int index);
    int getSpellCount() { return static_cast<int>(spells.size());
}
    std::string toString();
    std::string getSpellName(const int index) { return spells[index]->getName();
}
    BasicSpell::Type getSpellType(const int index) { return spells[index]->getType(); }
    BasicSpell* getSpell(int index);
    bool doesSpellReach(const int index, const Coordinates& caster, const Coordinates& target) {
        return spells[index]->doesSpellReach(caster, target);
}

    json toJson() const;
    void fromJson(const json& j);
};
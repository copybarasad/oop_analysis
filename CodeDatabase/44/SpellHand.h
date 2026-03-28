#pragma once
#include <vector>
#include <memory>
#include "Spell.h"

class SpellHand {
public:
    explicit SpellHand(int max_hand_size = 5);

    int GetSpellCount() const;
    const std::vector<std::unique_ptr<Spell>>& GetSpells() const;
    bool IsEmpty() const;

    void AddSpell(const std::string& name);
    void AddRandomSpell();
    bool ShowSpells() const;
    bool UseSpell(size_t index,
                  std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
                  int px, int py, Field& field);
    void RemoveHalfSpells();
    void Clear();

private:
    std::vector<std::unique_ptr<Spell>> spells_;
    int max_hand_size_;
};
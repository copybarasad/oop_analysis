#ifndef HAND_HPP
#define HAND_HPP

#include <cstddef>
#include <vector>

#include "../field/field.hpp"
#include "../map/map.hpp"
#include "../player.hpp"
#include "../save/dumpable.hpp"
#include "spell.hpp"

class Hand : public Dumpable {
  public:
    Hand(size_t capacity);
    void addSpell(Spell *spell);
    void addUpgrades(int upgrades);
    bool canCast(int index, Positionable *caster, Map *map, Field *target);
    void cast(int index, Positionable *caster, Map *map, Field *target);
    size_t len();
    size_t cap();
    void removeHalf();

    bytedump dump() override;
    Hand(bytedump bytes);

    std::string toString();

  private:
    std::vector<Spell *> m_Spells;
    size_t m_Capacity;
    int m_Upgrades;
};

#endif /* HAND_HPP */

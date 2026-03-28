#ifndef UNTITLED_SPELLFACTORY_H
#define UNTITLED_SPELLFACTORY_H

#include <memory>
#include <string>
#include <optional>

struct Coord;
class Field;
class Player;
class Hand;
class Spell;

struct UpgradeData {
    int addRange = 0;
    int addW = 0;
    int addH = 0;
    int addTrap = 0;
    int addSummon = 0;
    bool empty() const noexcept {
        return addRange == 0 && addW == 0 && addH == 0 && addTrap == 0 && addSummon == 0;
    }
};

class SpellFactory {
public:
    static std::shared_ptr<Spell> create(const std::string& name,
                                         Field& field,
                                         Coord casterPos,
                                         std::optional<Coord> target,
                                         std::shared_ptr<Player> caster,
                                         bool applyUpgrades,
                                         const UpgradeData& upgrades,
                                         Hand* handPtr);
};

#endif

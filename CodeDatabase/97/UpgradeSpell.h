#ifndef UNTITLED_UPGRADESPELL_H
#define UNTITLED_UPGRADESPELL_H

#include "Spell.h"
#include "Coord.h"
#include <memory>
#include <optional>

class Field;
class Player;
class Hand;

class UpgradeSpell : public Spell {
public:
    UpgradeSpell(int addRange, int addWidth, int addHeight, int addTrap, int addSummon,
                 Field& field, Coord casterPos, std::optional<Coord> target,
                 std::shared_ptr<Player> caster, Hand* hand);

    ~UpgradeSpell() override = default;

    bool apply() override;
    std::shared_ptr<Spell> clone() const override;
    std::string name() const override;
    std::string description() const override;


private:
    int addRange_;
    int addW_;
    int addH_;
    int addTrap_;
    int addSummon_;

    Field* field_;
    Coord casterPos_;
    std::optional<Coord> target_;
    std::shared_ptr<Player> caster_;
    Hand* hand_;
};

#endif // UNTITLED_UPGRADESPELL_H

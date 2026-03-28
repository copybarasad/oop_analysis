#ifndef UNTITLED_AREADAMAGESPELL_H
#define UNTITLED_AREADAMAGESPELL_H

#include "Spell.h"
#include "Coord.h"
#include <memory>
#include <optional>

class Field;
class Player;

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int dmg, int range, int w, int h,
                    Field& field,
                    Coord casterPos,
                    std::optional<Coord> target,
                    std::shared_ptr<Player> caster);

    ~AreaDamageSpell() override = default;

    bool apply() override;
    std::shared_ptr<Spell> clone() const override;
    std::string name() const override;
    std::string description() const override;

private:
    int dmg_;
    int range_;
    int w_;
    int h_;
    Field* field_;
    Coord casterPos_;
    std::optional<Coord> target_;
    std::shared_ptr<Player> caster_;
};

#endif // UNTITLED_AREADAMAGESPELL_H


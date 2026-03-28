#ifndef UNTITLED_TRAPSPELL_H
#define UNTITLED_TRAPSPELL_H

#include "Spell.h"
#include "Coord.h"
#include <memory>
#include <optional>

class Field;
class Player;

class TrapSpell : public Spell {
public:
    TrapSpell(int dmg, int range,
              Field& field,
              Coord casterPos,
              std::optional<Coord> target,
              std::shared_ptr<Player> caster);

    ~TrapSpell() override = default;

    bool apply() override;
    std::shared_ptr<Spell> clone() const override;
    std::string name() const override;
    std::string description() const override;

private:
    int dmg_;
    int range_;
    Field* field_;
    Coord casterPos_;
    std::optional<Coord> target_;
    std::shared_ptr<Player> caster_;
};

#endif // UNTITLED_TRAPSPELL_H


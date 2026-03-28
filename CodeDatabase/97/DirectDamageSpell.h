#ifndef UNTITLED_DIRECTDAMAGESPELL_H
#define UNTITLED_DIRECTDAMAGESPELL_H

#include "Spell.h"
#include "Coord.h"
#include <memory>
#include <optional>

class Field;
class Player;

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int dmg, int range,
                      Field& field,
                      Coord casterPos,
                      std::optional<Coord> target,
                      std::shared_ptr<Player> caster);

    ~DirectDamageSpell() override = default;

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

#endif // UNTITLED_DIRECTDAMAGESPELL_H

#ifndef ISPELL_H
#define ISPELL_H

#include <string>

class Player;
class GameController;

enum class SpellType {
    FIREBALL,
    ZAP,
    TRAP,
    SUMMON,
    BUFF
};

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool cast(Player& caster, GameController& game) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual SpellType getType() const = 0;
};

#endif
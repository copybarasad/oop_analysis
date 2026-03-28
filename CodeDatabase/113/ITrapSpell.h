#ifndef ITRAPSPELL_H
#define ITRAPSPELL_H

#include <string>
#include "GameObject.h"

class GameField;
class Player;

class ITrapSpell {
public:
    virtual ~ITrapSpell() = default;

    // Специфичные методы для заклинаний-ловушек
    virtual int getTrapDamage() const = 0;
    virtual bool canPlaceTrap(const Player& caster, const GameField& field, int x, int y) const = 0;
    virtual void onTrapTriggered(int x, int y, GameObject& trigger) const = 0;
    virtual std::string getTrapType() const = 0;
};

#endif // ITRAPSPELL_H
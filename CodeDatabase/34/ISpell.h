#ifndef GAME_ISPELL_H
#define GAME_ISPELL_H

#include <string>
#include <iostream>
#include "../../utils/Point.h"

namespace Game {

class Game;

// ID для сериализации
enum class SpellType {
    DirectDamage,
    AreaDamage,
    Trap,
    Summon,
    Enhancement
};

class ISpell {
public:
    virtual ~ISpell() = default;

    virtual bool cast(Game& game, const Utils::Point& target, int enhancementStacks) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool requiresTarget() const = 0;
    
    // Новые методы для сохранения/загрузки
    virtual SpellType getType() const = 0;
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
};

}

#endif
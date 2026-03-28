//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_SPELL_H
#define GAME_TERM_SPELL_H

#include <string>

#include "../utils /ISerializable.h"

class Field;

enum class Type { DirectDamage, AreaDamage, Trap };

class ISpell: public ISerializable{
public:
    virtual Type getType() const = 0;
    virtual ~ISpell() = default;
    virtual std::string getName() const = 0;
    virtual void cast(Field & field) = 0;
};


#endif //GAME_TERM_SPELL_H
#pragma once

#include <string>

class Player;
class Field;

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    //HEAL,
    SPELL_TYPE_COUNT,
};

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool use(Field& field, Player* caster, int targetX, int targetY) = 0;
    virtual std::string getName() = 0;
    virtual int getRange() = 0;
    virtual int getDamage() = 0;
    virtual SpellType getType() = 0;
    virtual bool isInRange(int casterX, int casterY, int targetX, int targetY) = 0;
    virtual int getAreaSize() = 0;

#if 0
    bool isInRange(int casterX, int casterY, int targetX, int targetY) {
        int dx = targetX - casterX;
        int dy = targetY - casterY;
        return (dx * dx + dy * dy) <= (range * range);
    }
#endif
};

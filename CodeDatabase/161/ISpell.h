#ifndef ISPELL_H
#define ISPELL_H

#include <memory>

#include "../Entity.h"
#include "../../logic/Tokens.h"

class Field;
class Cell;
class Hand;

enum class SpellType {
    None,
    AreaDmg,
    DirDmg,
    Enhancement,
    Summon,
    Trap,
};

struct SpellContext {
    std::shared_ptr<Entity> caster;
    std::shared_ptr<Entity> target;
    Field* field = nullptr;
    std::shared_ptr<Cell> cell;
	int base_x, base_y;
	std::shared_ptr<Hand> hand;
};

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual SpellType getSpellType() const = 0;
    virtual void upgrade(int) = 0;
    virtual bool use(const SpellContext&) = 0;
	virtual TokenSpell serialise() const = 0;
};

#endif // ISPELL_H
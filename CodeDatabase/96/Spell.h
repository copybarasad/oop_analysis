#pragma once
#include <vector>
#include <utility>
#include "Field.h"
#include "SpellContext.h"
#include "struct/dataSpell.h"
#include "struct/SpellType.h"


class Spell {
public:
    virtual ~Spell() = default;
    virtual bool Cast(SpellContext& ctx) = 0;
    virtual void enhance() = 0;

    virtual std::shared_ptr<dataSpell> Save() = 0;

};
#pragma once

#include "IEntity.hpp"
#include "ISpell.hpp"
#include <memory>

class IHasHand {
public:
    virtual std::shared_ptr<ISpell> get_spell(int index) = 0;
    virtual bool add_spell(std::shared_ptr<ISpell> spell) = 0;
};
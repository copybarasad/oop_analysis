//
// Created by Artem on 24.10.2025.
//

#include "BuffSpellSpell.h"

std::unique_ptr<Spell> BuffSpellSpell::clone() const {
    return std::make_unique<BuffSpellSpell>(*this);
}

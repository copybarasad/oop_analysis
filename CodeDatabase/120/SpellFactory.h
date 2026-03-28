#pragma once

#include "AoESpell.h"
#include "BuffSpell.h"
#include "SummonSpell.h"
#include "TrapSpell.h"

class SpellFactory {
 public:
  std::unique_ptr<ISpell> CreateSpell(const std::string& name);
};

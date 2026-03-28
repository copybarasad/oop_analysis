#pragma once
#include <memory>
#include "zap_spell.hpp"
#include "fireball_spell.hpp"
#include "spawn_enemy_spell.hpp"
#include "summon_spell.hpp"
#include "trap_spell.hpp"

class SpellFactory {
public:
    static std::shared_ptr<ISpell> create(char c);
};

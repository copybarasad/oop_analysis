#pragma once

#include <memory>
#include <random>

#include "gameconfig.h"
#include "ispell.h"

namespace game {
    struct SpellFactory {
        static std::unique_ptr<ISpell> createRandom(std::mt19937& rng,
                                                    const GameConfig& cfg);
        
        static std::unique_ptr<ISpell> createById(SpellId id,
                                                  const GameConfig& cfg);
    };
}
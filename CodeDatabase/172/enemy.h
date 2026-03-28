#pragma once

#include "cell.h"
#include "character.h"
#include "gameconfig.h"

#include <random>
#include <vector>

namespace game {
    /// @brief Enemy entity (<- Character)
    class Enemy final: public Character {     
        private:
            bool canMoveTo(Pos position, int w, int h,
                           const std::vector<std::vector<Cell>>& cells) const ;
        
        public:
            // === Constructor ===
            Enemy(const GameConfig& cfg, Pos spawnPos)
                : Character(cfg.enemyDefaultHP, cfg.enemyDefaultDamage,
                            spawnPos) {}

            Enemy(const Enemy&) = default;
            Enemy(Enemy&&) = default;
            Enemy& operator=(const Enemy&) = default;
            Enemy& operator=(Enemy&&) = default;
            


            // === Movement ===
            Pos moveRandom(std::mt19937& rng,
                            int w, int h,
                            Character& player,
                            const std::vector<std::vector<Cell>>& cells,
                            const std::vector<Pos>& occupiedPositions);

            Pos moveToPlayer(int w, int h,
                              Character& player,
                              const std::vector<std::vector<Cell>>& cells,
                              const std::vector<Pos>& occupiedPositions);
    };
}
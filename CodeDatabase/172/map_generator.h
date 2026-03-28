#pragma once

#include "map_data.h"
#include "gameconfig.h"

#include <random>

namespace game {
    class MapGenerator {
        private:
            MapData map_;

            void generateWalls(const GameConfig& cfg, std::mt19937& rng);
            void generateSlimes(const GameConfig& cfg, std::mt19937& rng);

            void placeEnemyBuilding(const GameConfig& cfg);
            void placeEnemyTower(const GameConfig& cfg);
            bool inBounds(Pos p, int shift) const;

        public:
            MapData generateMap(int w, int h, const GameConfig& cfg, std::mt19937& rng);

            #ifdef TESTING
            MapData generateWallTestMap(int w, int h, const GameConfig& cfg, std::mt19937& rng);
            MapData generateSlimeTestMap(int w, int h, const GameConfig& cfg, std::mt19937& rng);
            #endif
    };
}
#pragma once

#include "pos.h"

namespace game {
    class EnemyBuilding {
        private:
            Pos position_{};
            int spawnInterval_{16};
            int enemiesPerSpawn_{1};

            int spawnTimer_{1};
        
        public:
            // == Constructor ==
            EnemyBuilding(Pos pos, int interval, int count)
                : position_(pos), spawnInterval_(interval),
                  enemiesPerSpawn_(count), spawnTimer_(1) {}

            // == Getters ==
            Pos getPosition()       const { return position_; }
            int getTimer()          const { return spawnTimer_; }
            int turnsRemainSpawn()  const { return spawnInterval_ - spawnTimer_; }
            int getCountEnemies()   const { return enemiesPerSpawn_; }

            // == Setters ==
            void setTimer(int timer)        { spawnTimer_ = timer; }
            void setCountEnemies(int count) { enemiesPerSpawn_ = count; }
            
            // == Gameplay ==
            void incrementTimer()   { spawnTimer_++; }
            void resetTimer()       { spawnTimer_ = 0; }
            bool shouldSpawn()      { return spawnTimer_ % spawnInterval_ == 0; }
    };
}
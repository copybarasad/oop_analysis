#pragma once

#include "cell.h"
#include "ebuilding.h"
#include "enemy.h"
#include "etower.h"
#include "player.h"
#include "gameconfig.h"
#include "map_data.h"

#include "error.h"

#include <vector>
#include <random>
#include <optional>

namespace game {
    // ------------------ Field of game -------------------
    class Field {
        private:
            // === Size ===
            int width_;
            int height_;

            // === Game parameters ===
            GameConfig cfg_;

            // === Single random number generator ===
            std::mt19937 rng_;

            // === Mutable parameters ===
            int turnCounter_;

            // === Cells, entities and buildings ===
            std::vector<std::vector<Cell>> cells_;
            Player player_;
            std::vector<Enemy> enemies_;
            std::vector<EnemyBuilding> ebuildings_;
            ETower etower_;

            bool inBounds(Pos p, int shift) const;

            // == Gameplay ==
            void createCorpse(Character& enemy);
            void moveEnemies();
            bool spawnEnemyNearBuilding(EnemyBuilding& ebuilding);

        public:

        // === Constructor ===
        Field(int w, int h, const GameConfig& cfg, std::mt19937& rng);
        
        // === Constructor for edit map ===
        #ifdef TESTING
        Field(const GameConfig& cfg, MapData& md, std::mt19937& rng);
        #endif

        // === Rule of Five ===
        Field(const Field& other);
        Field(Field&& other);
        Field& operator=(const Field& other);
        Field& operator=(Field&& other);
        ~Field() = default;
        
        // == Helpers ==
        int enemyIndexAt(Pos p) const;
        bool hasEnemyAt(Pos p) const;

        // == Spells ==
        bool damageEnemyAt(Pos p, int dmg);
        void damagePlayer(int dmg) { player_.takeDamage(dmg); }
        bool tryCastByIndex(size_t index, std::optional<Pos> target);
        std::optional<Pos> findNearestEnemy(Pos from, int maxRange = -1) const;
        bool applyAreaDamage(Pos center, int radius, int dmg);
        bool applyAreaDamage2x2(Pos topLeft, int dmg);
        bool placeTrap(Pos p, int dmg);
        bool triggerTraps();

        // === Getters ===
        int getW()              const { return width_; }
        int getH()              const { return height_; }
        char iconAt(Pos p)      const;

        const Cell& getCell(int x, int y)       const { return cells_[y][x]; }
        const Player& getPlayer()               const { return player_; }
        const std::vector<Enemy>& getEnemies()  const { return enemies_; }
        const std::vector<EnemyBuilding>& getEBuildings() const { return ebuildings_; }
        const ETower& getETower()               const { return etower_; }

        int getTurn()           const { return turnCounter_; }
        int getNextSpawnTurn()  const;
        bool isPlayerAlive()    const { return player_.isAlive(); }
        int enemiesAlive()      const;
        
        // === Gameplay ===
        bool movePlayer(Direction dir);
        void nextTurn();
        bool playerAttack();
        void switchPlayerFightMode();

        // === Synchronization ===
        void setPlayerHand(const PlayerHand& hand) { player_.setHand(hand); }
        void save(std::ostream& out) const;
        void load(std::istream& in);

        // === Testing ===
        #ifdef TESTING
        void spawnEnemy(int x, int y);
        void placeWall(int x, int y);
        #endif
    };

}
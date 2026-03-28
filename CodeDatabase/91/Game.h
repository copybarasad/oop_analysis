#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "Spells/ISpell.h"
#include "Towers/Tower.h"
#include "Commands/ICommand.h"
#include "Input/IInputHandler.h"
#include "Renderers/IGameRenderer.h"
#include "Logger.h"
#include <vector>
#include <memory>
#include <random>

enum class GameResult {
    WIN,
    LOSE,
    QUIT
};

class Game {
    private:
        std::unique_ptr<Player> player;
        std::vector<Enemy> enemies;
        GameField field;
        bool gameRunning;
        bool game_won_;
        int turnCount;

        std::vector<std::unique_ptr<ITower>> towers;
        std::vector<std::pair<Position, int>> activeTraps;

        Logger* logger_;

        void processEnemyTurns();
        void processTowerTurns();
        bool processSpellSelection();
        bool processBuySpell();
        bool processRangedAttack();

        void checkCombat();
        void spawnEnemies(int enemyCount = 3);

        bool hasLineOfSight(const Position& from, const Position& to) const;

        void removeDeadEnemies();
        void checkGameState();

    public:
        Game(int fieldWidth = 15, int fieldHeight = 15, Logger* logger = nullptr);
        bool processPlayerTurn(char input);

        // === Main Gameplay Cycle ===
        GameResult run(IInputHandler& input_handler, IGameRenderer* renderer = nullptr);

        // === Spells ===
        bool castSpell(int spellIndex, const Position& target);
        void addTrap(const Position& position, int damage);
        void removeTrap(const Position& position);
        void damagePlayer(int damage);
        bool damageEntityAt(const Position& pos, int damage);
        bool canTargetForDamage(const Position& position) const;
        bool canTargetForAreaSpell(const Position& center, int radius, int areaSize) const;
        void damageEnemiesInArea(const std::vector<Position>& area, int damage);
        bool canTargetForTrap(const Position& position) const;
        void placeTrapAt(const Position& position, int damage);

        // === Getters ===
        const Player& getPlayer() const;
        const std::vector<Enemy>& getEnemies() const;
        const GameField& getField() const;
        int getTurnCount() const;

        const std::vector<std::unique_ptr<ITower>>& getTowers() const;
        const std::vector<std::pair<Position, int>>& getTraps() const;
        bool hasTrapAtPosition(const Position& position) const;
        bool hasTowerAtPosition(const Position& position) const;
        bool isPlayerInRange(const Position& position, int range) const;
        const Enemy* findEnemyAtPosition(const Position& position) const;
        Enemy* findEnemyAtPosition(const Position& position);

        Player& getPlayerRef();
        void setPlayerState(Player new_player);
        std::vector<Enemy>& getEnemiesRef();
        std::vector<std::unique_ptr<ITower>>& getTowersRef();

        void executeCommand(std::unique_ptr<ICommand> command);
        void executeSaveLogic();

        // === JSON ===
        void loadState(const nlohmann::json& j);
};

#endif
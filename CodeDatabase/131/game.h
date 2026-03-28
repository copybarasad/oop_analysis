#ifndef OOP_GAME_H_
#define OOP_GAME_H_

#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "trap.h"
#include "ally.h"
#include "enemy_tower.h"
#include "entity_manager.h"
#include "turn_manager.h"
#include "spell_context.h"
#include "game_memento.h"
#include "event_manager.h"
#include <vector>
#include <string>
#include <memory>

namespace rpg {
    class LevelManager;
    class SaveManager;
    class UpgradeSystem;
    template<typename T> class GameVisualizer;
    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight
    };

    enum class GameCommand {
        kMove,
        kSwitchCombatMode,
        kAttack,
        kCastSpell,
        kBuySpell,
        kQuit,
        kInvalid
    };

    class Game final : public DamageSpellContext,
                       public TrapSpellContext,
                       public SummonSpellContext,
                       public EnhancementSpellContext {
    public:
        static constexpr Position kPlayerStartPosition{0, 0};

        static constexpr int kPlayerStartHealth = 100;
        static constexpr int kPlayerMeleeDamage = 20;
        static constexpr int kPlayerRangedDamage = 15;

        static constexpr int kScorePerKill = 50;

        Game();
        ~Game();

        void StartNewGame();
        void LoadGameFromFile(const std::string& filename = "savegame.dat");
        
        explicit Game(size_t field_width, size_t field_height);

        [[nodiscard]] bool IsRunning() const;

        [[nodiscard]] bool IsPlayerAlive() const;

        [[nodiscard]] const Player &GetPlayer() const;

        [[nodiscard]] int GetTurnNumber() const;

        [[nodiscard]] std::string ProcessCommand(GameCommand command, Direction dir = Direction::kUp);

        [[nodiscard]] std::string SwitchPlayerCombatMode();

        [[nodiscard]] std::string AttackNearby();

        [[nodiscard]] std::string CastSpell(size_t spell_index, const Position &target_pos);

        [[nodiscard]] std::string BuyRandomSpell();
        
        [[nodiscard]] std::string SaveGame();

        void Quit();

        void RestorePlayer(const Player& player);

        void SetTurnNumber(int turn_number);

        Player& GetMutablePlayer();

        void AddEnemy(int health, int damage, const Position &pos);

        void AddEnemyBuilding(const Position &pos, int spawn_interval,
                              int enemy_health, int enemy_damage);

        void AddTower(const Position &pos);

        [[nodiscard]] GameField &GetField();

        [[nodiscard]] std::vector<EnemyTower> &GetTowers();

        [[nodiscard]] const std::vector<EnemyTower> &GetTowers() const;

        [[nodiscard]] Position GetPlayerPosition() const override;

        void DamagePlayer(int damage);

        [[nodiscard]] const GameField &GetField() const override;

        [[nodiscard]] std::vector<Enemy> &GetEnemies() override;

        [[nodiscard]] std::vector<EnemyBuilding> &GetBuildings() override;

        [[nodiscard]] std::vector<Trap> &GetTraps() override;

        [[nodiscard]] std::vector<Ally> &GetAllies() override;

        [[nodiscard]] const std::vector<Enemy> &GetEnemies() const override;

        [[nodiscard]] const std::vector<EnemyBuilding> &GetBuildings() const override;

        [[nodiscard]] const std::vector<Trap> &GetTraps() const override;

        [[nodiscard]] const std::vector<Ally> &GetAllies() const override;

        void ActivateEnhancement() override;
        
        [[nodiscard]] GameMemento CreateMemento() const;
        void RestoreFromMemento(const GameMemento& memento);
        
        [[nodiscard]] int GetCurrentLevel() const;
        void SetCurrentLevel(int level);
        
        [[nodiscard]] EventManager& GetEventManager() { return event_manager_; }
        [[nodiscard]] const EventManager& GetEventManager() const { return event_manager_; }
        
        [[nodiscard]] bool ShouldContinueLevel() const;
        void CheckAndHandleLevelCompletion();
        void CheckAndHandleGameOver();

    private:
        std::unique_ptr<LevelManager> level_manager_;
        std::unique_ptr<SaveManager> save_manager_;
        std::unique_ptr<UpgradeSystem> upgrade_system_;
        std::unique_ptr<GameVisualizer<class ConsoleRenderer>> visualizer_;

        GameField field_;
        Player player_;
        EntityManager entity_manager_;
        TurnManager turn_manager_;
        EventManager event_manager_;
        int turn_number_;
        bool is_running_;

        void InitializeLevel(const struct LevelConfig& config);
        void PopulateLevelWithEnemies(const struct LevelConfig& config);
        void PopulateLevelWithBuildings(const struct LevelConfig& config);
        void PopulateLevelWithTowers(const struct LevelConfig& config);
        void CheckLevelCompletion();
        void AdvanceToNextLevel();
        void ShowUpgradeMenu();
        void HandleGameOver();

        [[nodiscard]] std::string ProcessEnemyTurns();

        void ProcessBuildingTurns();

        void ProcessTowerTurns();

        [[nodiscard]] std::string ProcessAllyTurns();

        [[nodiscard]] std::string CheckTraps();

        void RemoveDeadEnemies();

        void RemoveDeadAllies();

        [[nodiscard]] bool IsPositionOccupiedByEnemy(const Position &pos) const;

        [[nodiscard]] bool IsPositionOccupiedByBuilding(const Position &pos) const;

        [[nodiscard]] bool IsPositionOccupiedByPlayer(const Position &pos) const;

        [[nodiscard]] Enemy *FindEnemyAt(const Position &pos);

        void EndTurn();
        
        void NotifyEvent(GameEventType type, const std::string& description,
                        const std::map<std::string, std::string>& data = {});

        // Инвариант: turn_number_ >= 0
    };
}

#endif

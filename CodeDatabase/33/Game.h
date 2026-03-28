#pragma once
#include <vector>
#include <random>
#include <string>
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "Tower.h"
#include "Turret.h"
#include "Trap.h"
#include "PlayerHand.h"
#include <iosfwd>
#include "LevelConfig.h"
#include "GameEventBus.h"
#include "Command.h"

template <typename InputReader, typename Renderer> class GameController;
class GameVisualizerBase;

class Game {
public:
    enum class Difficulty { Easy, Normal, Hard, Nightmare };

    Game(const LevelConfig& cfg, Difficulty diff, std::size_t hand_capacity);
    
    enum class LevelResult { Victory, Defeat, Quit };

    void NewGame(const std::string& player_name, std::size_t hand_capacity);
    void ReconfigureFrom(const LevelConfig& cfg);
    void StartLevel();

    bool LastLevelWasVictory() const;
    int  CurrentLevelIndex() const;
    void SetCurrentLevelIndex(int idx);
    void HealPlayerFull();
    void HalveHandRandom();
    void ApplyUpgradeChoice(int id);

    void SaveToStream(std::ostream& os) const;
    void LoadFromStream(std::istream& is);

    bool TrySaveCommand(const std::string& path);
    bool TryLoadCommand(const std::string& path);

    bool SpellDirectDamageRay(int dx, int dy, int range, int damage);
    void AreaDamageAt(Coord top_left, int size, int damage);
    bool PlaceTrapAdj(int dx, int dy, int damage);
    bool SummonAlliesAroundPlayer(int count, int ally_hp, int ally_damage);

    void SetEventBus(GameEventBus* bus);
    const Field& GetField() const;
    const Player& GetPlayer() const;
    const std::vector<Enemy>& Enemies() const;
    const std::vector<Ally>& Allies() const;
    const std::vector<Tower>& Towers() const;
    const std::vector<Turret>& Turrets() const;
    const std::vector<Trap>& Traps() const;
    const PlayerHand& GetHand() const;
    bool MoveLockedNextTurn() const;
    bool QuitRequested() const;

    void RequestQuit();
    void ResetLevelFlags();
    void MarkVictory();

    void ToggleMode();
    bool CastSpellByIndex(int idx);

    void CleanupDead();
    void MaybeRewardSpell();
    void AlliesTurn();
    void EnemiesTurn();
    void TurretsTurn();
    void TrySpawnFromTowers();

    template <typename InputReader, typename Renderer>
    friend class GameController;

private:
    bool last_victory_ = false;
    int  current_level_index_ = 1;
    bool quit_requested_ = false;
    
    bool RayDamageFromPlayer(int dx, int dy, int range, int damage, bool from_spell);
    void SpellAreaDamageSquare(Coord top_left, int size, int damage);
    bool SpellPlaceTrap(int dx, int dy, int damage);
    bool SpellSummonAllies(int count, int ally_hp, int ally_damage);

    
    struct Balance {
        int player_hp;
        int player_melee;
        int player_ranged;
    };

    static Balance MakeBalance(Difficulty d);

    static Field  CreateFieldChecked(int w, int h, std::mt19937& rng);
    
    std::mt19937 rng_;
    Difficulty   diff_;
    Field        field_;
    Player       player_;
    PlayerHand   hand_;
    GameEventBus* event_bus_ = nullptr;

    int w_;
    int h_;
    int init_enemies_;
    int enemy_hp_;
    int enemy_damage_;
    int init_towers_;
    int tower_interval_;
    int init_turrets_;
    int turret_hp_;
    int turret_damage_;
    int turret_range_;
    int turret_cooldown_;

    std::vector<Enemy>  enemies_;
    std::vector<Ally>   allies_;
    std::vector<Tower>  towers_;
    std::vector<Turret> turrets_;
    std::vector<Trap>   traps_;

    bool move_locked_next_turn_ = false;
    int  enemies_killed_ = 0;
    int  last_reward_kill_count_ = 0;
    
    bool IsBlocked(const Coord& p) const;
    void PlacePlayerAndEnemies();
    void PlaceTowers();
    void PlaceTurrets();
    void Notify(const GameEvent& event);

    void TryMovePlayer(int dx, int dy);
    void PlayerAttack(int dx, int dy);

    bool IsEnemyAt(Coord p) const;
    int  EnemyIndexAt(Coord p) const;

    bool IsAllyAt(Coord p) const;
    int  AllyIndexAt(Coord p) const;

    bool IsTowerAt(Coord p) const;
    int  TowerIndexAt(Coord p) const;

    bool IsTurretAt(Coord p) const;
    int  TurretIndexAt(Coord p) const;

    bool IsTrapAt(Coord p) const;
    int  TrapIndexAt(Coord p) const;
    void RemoveTrapByIndex(int trap_idx);

};

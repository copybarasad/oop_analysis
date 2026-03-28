#ifndef GAME_HPP
#define GAME_HPP
#include "board.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "trap.hpp"
#include "tower.hpp"
#include <vector>
#include <memory>
#include <string>
#include <random>
class ISpell;
class Input;
struct SaveData;
class EventBus;
class Game {
public:
    Game(int width,int height,double wall_density,
         const std::string& player_name,int player_hp,int player_melee,int player_ranged,
         int enemies_count,int enemy_hp,int enemy_damage,unsigned seed=0);
    ~Game();
    bool PlayerTurn(const std::string& command, Input& input);
    bool EnemiesTurn();
    bool IsPlayerDead() const noexcept;
    bool IsVictory() const noexcept;
    const Board& BoardRef() const noexcept;
    const std::vector<Enemy>& Enemies() const noexcept;
    const std::vector<EnemyTower>& Towers() const noexcept;
    const std::vector<Trap>& Traps() const noexcept;
    void PlayerPos(int& x,int& y) const noexcept;
    bool DamageEnemyAt(int x,int y,int dmg);
    bool DamageTowerAt(int x,int y,int dmg);
    bool EnemyAt(int x,int y) const;
    bool HasTrapAt(int x,int y) const;
    void PlaceTrap(int x,int y,int dmg);
    void DamagePlayer(int dmg, const std::string& source, int sx, int sy);
    void CurrentActingTowerPos(int& x,int& y) const noexcept;
    void SetEventBus(EventBus* bus) noexcept;
    void StartLevel(int width,int height,double wall_density,
                    int enemies_count,int enemy_hp,int enemy_damage,unsigned seed);
    void SaveState(SaveData& data) const;
    void LoadState(const SaveData& data);
    void SetSpellPowerBonus(int bonus);
    int SpellPowerBonus() const noexcept;
    void DiscardHalfHand(std::mt19937& rng);
    Player& PlayerRef() noexcept;
    const Player& PlayerRef() const noexcept;
private:
    Board board_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<Trap> traps_;
    std::vector<EnemyTower> towers_;
    int kills_since_spell_;
    int acting_tower_x_;
    int acting_tower_y_;
    double wall_density_;
    int spell_power_bonus_;
    std::mt19937 spell_rng_;
    EventBus* event_bus_;
    bool IsOccupiedByEnemy(int x,int y, std::size_t* out_index=nullptr) const;
    Point FindAnyFloor(unsigned seed) const;
    std::vector<int> BuildDistanceFrom(const Point& start) const;
    std::vector<Point> LargestConnectedComponent() const;
    void PlaceActorsInLargestComponent(int enemies_count,int enemy_hp,int enemy_damage,unsigned seed);
    void PlaceOneTowerInComponent(unsigned seed);
    bool PlayerAttack(int dx,int dy);
    void CleanupDeadEnemies();
    void TowersTurn();
    void MaybeGrantSpell();
    std::unique_ptr<ISpell> MakeRandomSpell();
    void InitializeLevel(int width,int height,double wall_density,
                         int enemies_count,int enemy_hp,int enemy_damage,unsigned seed,
                         bool give_initial_spell);
};
#endif

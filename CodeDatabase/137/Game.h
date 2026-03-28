#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "GameField.h"
#include "Player.h"
#include "PlayerHand.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Spell.h"
#include "Command.h"
#include "GameEvent.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

class EventObserver;

class Game {
public:
    Game();
    ~Game();

    void StartNewGame();
    void PauseGame();
    void ResumeGame();
    void ProcessCommand(const Command& command);

    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);

    void NextLevel();
    void UpgradePlayer(int choice);

    GameState GetState() const;
    const GameField* GetField() const;
    const Player* GetPlayer() const;
    const PlayerHand* GetPlayerHand() const;
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const;
    const std::vector<std::unique_ptr<EnemyBuilding>>& GetEnemyBuildings() const;
    const std::vector<std::unique_ptr<EnemyTower>>& GetEnemyTowers() const;
    const std::vector<std::unique_ptr<Ally>>& GetAllies() const;
    int GetCurrentLevel() const;
    int GetEnemiesKilled() const;

    void AddEventObserver(EventObserver* observer);
    void RemoveEventObserver(EventObserver* observer);

private:
    void ProcessPlayerAction(PlayerAction action, int spell_index = -1,
                            int target_x = -1, int target_y = -1);
    void EndPlayerTurn();
    void MoveEnemies();
    void MoveAllies();
    void UpdateEnemyBuildings();
    void UpdateEnemyTowers();
    void CheckLevelComplete();

    void InitializeLevel();
    void SpawnEnemies(int count);
    void SpawnEnemyBuildings(int count);
    void SpawnEnemyTowers(int count);
    void ClearLevel();

    int CalculateDistance(int x1, int y1, int x2, int y2) const;
    void ApplyEnhancementToSpell(Spell* spell);
    Spell* CreateRandomSpell();

    void NotifyEvent(const GameEvent& event);

    std::unique_ptr<GameField> field_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<PlayerHand> player_hand_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<EnemyBuilding>> enemy_buildings_;
    std::vector<std::unique_ptr<EnemyTower>> enemy_towers_;
    std::vector<std::unique_ptr<Ally>> allies_;

    GameState state_;
    int current_level_;
    int enemies_killed_;
    bool player_turn_ended_;

    std::vector<EventObserver*> event_observers_;
};

#endif // GAME_H

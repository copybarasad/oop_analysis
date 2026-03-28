#pragma once
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "SpellHand.h"
#include "TrapSpell.h"
#include "InputHandler.h"
#include "GameRenderer.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "KeyBindings.h"
#include <vector>
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

enum class RunResult {
    WIN,
    LOSE,
    QUIT
};

class Game {
public:
    Game(int width, int height, int player_health, int player_damage,
         int num_buildings, int player_x = 0, int player_y = 0);
    void Start();
    void RunLevel();

    void SaveGame(const std::string& filename) const;
    void LoadGame(const std::string& filename);

    Field& GetField() { return field_; }
    const Field& GetField() const { return field_; }
    Player& GetPlayer() { return player_; }
    const Player& GetPlayer() const { return player_; }
    SpellHand& GetSpellHand() { return spell_hand_; }
    const SpellHand& GetSpellHand() const { return spell_hand_; }

private:
    RunResult Run();
    bool HasRemainingEnemies();
    bool LoadGameMenu();
    int HandlePlayerAction(Command* cmd);
    void UpdateEnemies();
    std::pair<int, int> ChooseEnemyMove(Enemy& enemy) const;

    void RestartGame();
    void NextLevel();
    void LevelUpMenu();
    int GetSpellIndexFromPlayer();
    int GetSaveSlot(int save_count);
    std::string GetFileNameFromUser();

    int current_enemy_health_;
    int current_enemy_damage_;
    Field field_;
    Player player_;
    SpellHand spell_hand_;
    std::vector<Enemy> enemies_;
    int current_level_;
    bool game_started_;

    GameController<InputHandler> controller_;
    GameVisualizer<GameRenderer> visualizer_;

    void LoadKeyBindings();
};

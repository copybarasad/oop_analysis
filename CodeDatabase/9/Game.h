#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include <string.h>
#include <cstddef>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "Spell.h"
#include "SpellDirect.h"
#include "SpellAoE.h"
#include "TrapSpell.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "SummonSpell.h"
#include "EmpowerSpell.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "LevelManager.h"
#include "LevelUpMenu.h"
#include "Command.h"
#include "TurnSystem.h"


enum class UiMode{
    MainMenu,
    Settings,
    Playing,
    LevelUp
};


class Game{
public:
    void SaveDefault();
    void LoadDefault();
    Game(int width, int height, int totalLevels);
    UiMode uiMode() const{ return ui_mode_; }
    enum class GameState {
        Running,
        Victory,
        Defeat
    };

    const Board& board() const;
    void ApplyBoardSizeToWindow(sf::RenderWindow& window);

    const Player& player() const;

    const std::vector<Enemy>& enemies() const;
    const std::vector<EnemyTower>& towers() const;
    const std::vector<Ally>& allies() const;
    const EnemyBase* base() const;

    const LevelManager& levelManager() const;

    float cellWidth() const;
    float cellHeight() const;
    float panelWidth() const;

    const std::string& infoMessage() const;

    bool ApplyCommand(const Command& command);
    GameState state() { return state_; }
    void run();
    void update();

    Board& board() { return board_; }
    Player& player() { return player_; }
    
    std::vector<Enemy>& enemies() { return enemies_; }
    std::vector<EnemyTower>& towers() { return towers_; }
    std::vector<Ally>& allies() { return allies_; }

    EnemyBase* base() { return base_; }

    LevelManager& levelManager() { return level_manager_; }

    float cellWidth() { return cellW_; }
    float cellHeight() { return cellH_; }
    float panelWidth() { return panelW_; }

    std::string& infoMessage() { return info_message_; }

    MainMenu& mainMenu() { return main_menu_; }
    SettingsMenu& settingsMenu() { return settings_menu_; }
    LevelUpMenu& levelUpMenu() { return level_up_menu_; }
private:
    UiMode ui_mode_;
    int settings_board_width_;
    int settings_board_height_;
    Difficulty difficulty_;
    MainMenu main_menu_;
    SettingsMenu settings_menu_;

    void processEvents(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);


    bool isInside(std::pair<int, int> p);
    bool isEnemyAt(std::pair<int, int> p, int* IndexOut);

    void placeEntities(int enemyCount);
    void placeBase();

    void generateTerrain(int WallPercent, int slowPercent);

    void chekEnd();

    void initStartingSpell();
    bool castSelectedSpellAt(std::pair<int,int> cell);
    void grantRandomSpell();
    bool tryBuySpell();


    Board board_;
    Player player_;
    std::vector<Enemy> enemies_;

    EnemyBase* base_;

    std::mt19937 rng_;

    float cellW_;
    float cellH_;
    bool needEnemyStep_;

    GameState state_;
    sf::Text endText_;
    bool isAllyAt(std::pair<int,int> p, int* indexOut);

    float panelW_;

    std::vector<EnemyTower> towers_;

    std::vector<Ally> allies_;

    bool isTowerAt(std::pair<int,int> p, int* indexOut);

    void placeTower();

    void cleanupEnemies();
    void cleanupAllies();

    void awardEmpowerCardOrBuff();

    void startNewGame();
    


    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);

    std::string DifficultyToString() const;
    Difficulty DifficultyFromString(const std::string& value) const;
    LevelManager level_manager_;

    void setupLevelFromManager();
    void goToNextLevel();


    bool needResizeWindow_;

    LevelUpMenu level_up_menu_;
    void ApplyLevelUpChoice(LevelUpChoice choice);
    int CurrentPlayerDamage();

    int hp_bonus_;
    int melee_bonus_;
    int direct_spell_bonus_;


    std::string info_message_;
    TurnSystem turn_system_;
};



#endif
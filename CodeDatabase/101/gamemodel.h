#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "tower.h"
#include "enemy.h"
#include "map.h"
#include "player.h"
#include "spawnbuilding.h"
#include "observable.h"
#include "buttons.h"
#include "leon.h"
#include <ostream>
#include <istream>
#include "eventmanager.h"

enum OptionGame {
    mainMenu = 0,
    pauseMenu = 1,
    processGame = 2,
    upgradeMenu = 3
};

class GameModel : public Observable
{
private:
    Tower* enemyTower;
    SpawnBuilding* build;
    Map* mapGame;
    Player* user;
    Enemy* computer;
    std::vector<Leon*> myLeons;
    std::vector<Observer *> obs;
    Buttons* myButtons;
    int score;
    int flagAtack;
    bool m_inGame;
    OptionGame currentOption;
    int currentLevel;
    int playerLevel;
    int skillPoints;
    bool levelCompleted;
    float playerDamageMultiplier;
    float spellPowerMultiplier;
    int height;
    int width;
    int cell_height;
    int cell_width;

public:
    GameModel();
    ~GameModel();
    Player& get_player();
    Enemy& get_enemy();
    Tower& get_tower();
    Map& get_map();
    SpawnBuilding& get_build();
    std::vector<Leon*>& get_leons();
    Buttons& get_buttons();
    void add_leon(int x, int y);
    void set_score(int s);
    int get_score() const;
    std::vector<int> check_coords_map(int x, int y, int count, int type_character);
    bool get_inGame();
    int get_flagAtack();
    void change_flagAtack(int val);
    void change_inGame(bool val);
    void leon_journey();
    void hill(int xp, int x, int y);
    void player_close_attack();
    void player_ranged_attack();
    void enemy_journey();
    void enemy_attack();
    void tower_attack();
    void serialize(std::ostream &os) const ;
    void deserialize(std::istream &is) ;
    OptionGame get_current_option() const;
    void set_current_option(OptionGame option);
    int get_current_level() const;
    void set_current_level(int level);
    void create_level_map(int level);
    void strengthen_enemies(int level);
    void complete_level();
    bool is_level_completed() const;
    void reset_for_new_level();
    int get_player_level() const;
    int get_skill_points() const;
    void spend_skill_point();
    void upgrade_player_health();
    void upgrade_player_damage();
    void upgrade_spell_power();
    float get_player_damage_multiplier() const;
    float get_spell_power_multiplier() const;
    void reset_level_completed();
    int get_height();
    int get_width();
    int get_cell_height();
    int get_cell_width();

    void notifyPlayerMoved(int fromX, int fromY, int toX, int toY);
    void notifyPlayerAttacked(const std::string& target, int damage, int x, int y);
    void notifyPlayerDamaged(int damage, const std::string& source);
    void notifyPlayerHealed(int amount);
    void notifyEnemyAttacked(int damage);
    void notifyEnemyDamaged(int damage);
    void notifyEnemyDied(int x, int y);
    void notifyTowerDamaged(int damage);
    void notifyTowerDestroyed();
    void notifySpellCast(const std::string& spellName, int cost, int x, int y);
    void notifyMoneyChanged(int amount, int newBalance);
    void notifyScoreChanged(int newScore);
    void notifyCardReceived(const std::string& cardName);
    void notifyUpgradePurchased(const std::string& upgradeType, int cost);
    void notifyGameStarted();
    void notifyLevelStarted(int level);
    void notifyLevelCompleted();
};

#endif // GAMEMODEL_H

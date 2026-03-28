#pragma once
#include "GameConfig.h"
#include "Player.h"
#include "Sea.h"
#include "Enemy.h"
#include "Vortex.h"
#include "Octopus.h"
#include <vector>
#include <random>


class Game {
private:
    GameConfig config;
    Sea sea_field;
    Player pl;
    std::vector<Enemy> enemies;
    std::vector<Octopus> octopuses;
    std::vector<Vortex> vortexes;
    int defeated_enemies;
    bool isGameOn;

public:
    Game(GameConfig &cfg);

    void spawn();
    Position getFreePositions();
    Sea &getSea();
    Enemy* findEnemyAtPosition(Position &pos);
    Octopus* findOctAtPosition(Position &pos);
    void removeDeads();
    std::vector<Enemy>& getEnemies();
    Player &getPlayer();
    int getDefEns();

    template <typename DiverType>
    bool moveEntity(Position& new_pos, Position& curr_pos, DiverType &dv);
    GameConfig &getConfig();
    bool getIsGameOn();
    bool getIsWin();
    void plMovement(int dx, int dy);
    void plRangedAttack(int target_x, int target_y);
    void choiseSpell();
    Divers_bag& getPlayerBag();
    void useSpell(int spellIndex, int x, int y);
    void plSwitchAttackType();
    void enMovement();
    void octopus_attack();
    void resetPlayerTurn();
    void printStatistics();
    void printControls();
    void render();
    void reset(GameConfig &new_cfg);

    void save(const std::string& path) const;
    void load(const std::string& path);
};
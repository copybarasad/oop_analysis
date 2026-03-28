#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Pit.h"
#include "Cell.h"
#include "Player.h"
#include "Saloon.h"
#include "Barracks.h"
#include "Enemy.h"
#include "Teamate.h"
#include "CowboyTower.h"
#include "Trap.h"
#include "Names.h"
#include "Constants.h" 
#include <stdexcept>
#include <vector>
#include <memory>

using std::unique_ptr;



class GameField {
private:
    int width;
    int height;
    Cell** grid;

    int max_cnt_enemy;
    int max_cnt_pit;
    int max_cnt_barracks;
    int max_cnt_saloon;
    int max_cnt_cowboy_tower;
    int max_cnt_traps;


    void clearGrid();
    void copyGrid(const GameField& other);

public:
    std::vector<std::unique_ptr<Enemy>> arr_enemies;
    std::vector<std::unique_ptr<Pit>> arr_pit;
    std::vector<std::unique_ptr<Barracks>> arr_barracks;
    std::vector<std::unique_ptr<Saloon>> arr_saloon;
    std::vector<std::unique_ptr<CowboyTower>> arr_cowboy_towers;
    std::vector<std::unique_ptr<Trap>> arr_traps;
    std::vector<std::unique_ptr<Teamate>> arr_teamates;


    GameField(int w, int h);
    GameField(const GameField& other);
    GameField(GameField&& other);

    GameField& operator=(const GameField& other); 
    GameField& operator=(GameField&& other);

    ~GameField();

    void initializeField();
    void removeDeadObjects();
    
    Enemy& findEnemy(int x, int y);
    Saloon& findSaloon(int x, int y);
    Barracks& findBarracks(int x, int y);
    CowboyTower& findCowboyTower(int x, int y);
    Teamate& findTeamate(int x, int y);

    bool isValidPosition(int x, int y) const;
    bool canMoveTo(int x, int y) const;
    void setCellType(int x, int y, Type_cell type);
    void spawn(int count, Type_cell type);
    void clearCell(int x, int y);
    void activateTrap(int x, int y, Character& character, Player& Player);
    void processAllTurns(Player& player);

    //Геттеры
    int getWidth() const;
    int getHeight() const;
    int getEnemyCount() const;
    int getMaxCntEnemy() const;
    Cell& getCell(int x, int y) const;

    //Сеттеры
    void setMaxPitCnt(int newMaxCnt);

};

#endif
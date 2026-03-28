#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "cell.h"
#include "field_spell.h"
#include "trap.h"
#include "tower.h"

class Player;
class Enemy;
class BookOfSpells;

class GameField {
public:
    GameField(int size, BookOfSpells& book); 
    ~GameField() = default;
    
    bool canMoveTo(const Player& player, const std::vector<Enemy>& enemies, int x, int y) const;
    void display(const Player& player, const std::vector<Enemy>& enemies) const;
    bool inField(int x, int y) const;
    int getSize() const { return size; }

    FieldSpell* getSpellAtPosition(int x, int y);
    bool collectSpell(int x, int y);
    bool hasSpells() const;
    void respawnSpells(BookOfSpells& book);

    void addTrap(const Trap& trap);
    void checkTraps(std::vector<Enemy>& enemies);
    bool hasTrapAt(int x, int y) const;

    void addTower(const Tower& tower);
    void updateTowers(Player& player, std::vector<Enemy>& enemies);
    bool hasTowerAt(int x, int y) const;

    void saveState(std::ostream& file) const;
    void loadState(std::istream& file, BookOfSpells& book);

private:
    std::vector<std::vector<Cell>> field;
    std::vector<FieldSpell> spellsOnField;
    std::vector<Trap> traps;
    std::vector<Tower> towers;
    int size;
    void createSpells(BookOfSpells& book); 
    void createTowers();
    void initializeField();
};

#endif
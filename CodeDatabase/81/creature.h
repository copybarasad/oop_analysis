#ifndef CREATURE_H
#define CREATURE_H

#include "constants.h"

class Cell; 
class Field;
class Game;

class Creature{
protected:
    char emblem;
    int current_hp;
    int current_damage;
    int load_x;
    int load_y;
private:
    Cell* current_cell = nullptr;
    bool is_dead = false;
public:
    Creature(char e, int hp, int current_damage);
    int getIs_dead() const;
    int getDamage() const;
    void deal_damage(int damage);
    void set_current_cell(Cell* cell_ptr);
    int getLoadX() const;
    int getLoadY() const;
    Cell* get_current_cell() const;
    bool move(int dx, int dy, Field *game_field); 
    virtual void save(std::ofstream& ofs) const;
    virtual void load(std::ifstream& ifs);
    virtual ~Creature() = default;
    friend class Game;
};

#endif
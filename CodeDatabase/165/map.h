#ifndef MAPH
#define MAPH
#include <cstddef>
#include "./human/hero.h"
#include "./human/enemy.h"
#include "./building/castle.h"
#include <vector>
struct tile{
    char name;
    void* address;
};

class map{
    size_t height;
    size_t length;
    tile** tiles;
    hero* heroPtr;
    std::vector<enemy*> enemies;
    std::vector<castle*> castles;
    size_t turn;
    size_t difficultyLvL;
public:
    map(size_t, size_t, size_t);
    map(size_t, size_t);
    map();
    void generate(size_t, size_t, size_t, size_t);
    void generate(size_t, size_t, size_t);
    void generate(size_t, size_t);
    void generate(size_t);
    void generate();

    void show();
    
    size_t getHeight();
    size_t getLength();
    tile** getTiles();
    hero* getHero();
    void setHero(hero*);
    std::vector<enemy*>& getEnemies();
    std::vector<castle*>& getCastles();
    size_t getTurn();
    void nextTurn();
    void setTurn(size_t);
    size_t getDifficultyLvL();
    void setDifficultyLvl(size_t);
    ~map();
};
#endif
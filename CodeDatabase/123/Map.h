#ifndef MAP
#define MAP

// #include "PLayer.h"
// #include "Enemy.h"
// #include "Building.h"
#include "Character.h"
#include <vector>
#include <iostream>
using namespace std;

enum class CellType
{
    Simple,
    Slow,
    Block
};

struct Cell
{
    shared_ptr<Entity> ent;
    CellType type = CellType::Simple;
};

class Map
{
private:
    vector<vector<Cell>> mObjects;

public:
    Map(Vec2 size = Vec2(25, 25));

    Map(const Map &map);

    Map(Map &&map);

    void setSize(Vec2 size);

    Vec2 getSize() const;

    void putCharacter(shared_ptr<Entity> ch);

    vector<Cell> &operator[](size_t y)
    {
        return mObjects[y];
        // return mObjects[mObjects.size() - y - 1];
    }

    vector<Cell> &operator[](int y)
    {
        return mObjects[y];
        // return mObjects[mObjects.size() - y - 1];
    }

    Map &operator=(const Map &m);

    Map &operator=(Map &&m);

    void moveCharacterTo(shared_ptr<Character>, Vec2 dest);

    ~Map();
};

#endif
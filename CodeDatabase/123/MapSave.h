#ifndef MAP_SAVE
#define MAP_SAVE

#include "ISave.h"
#include "../GameSystem/Map.h"

class MapSave : public ISave
{
private:
    shared_ptr<Map> *map;

public:
    MapSave(shared_ptr<Map> *buff)
    {
        map = buff;
    }

    void write(fstream &file) override
    {
        Vec2 buff = map->get()->getSize();
        file.write(reinterpret_cast<char *>(&buff), sizeof(Vec2));

        for (size_t i = 0; i < map->get()->getSize().y; i++)
        {
            for (size_t j = 0; j < map->get()->getSize().x; j++)
            {
                file.write(reinterpret_cast<char *>(&map->get()->operator[](i)[j].type), sizeof(CellType));
            }
        }
    }

    void read(fstream &file) override
    {
        Vec2 buff;
        file.read(reinterpret_cast<char *>(&buff), sizeof(Vec2));
        *map = make_shared<Map>(buff);
        // map->get()->setSize(buff);
        for (size_t i = 0; i < map->get()->getSize().y; i++)
        {
            for (size_t j = 0; j < map->get()->getSize().x; j++)
            {
                CellType t;
                file.read(reinterpret_cast<char *>(&t), sizeof(CellType));
                map->get()->operator[](i)[j].type = t;
            }
        }
    }
};

#endif
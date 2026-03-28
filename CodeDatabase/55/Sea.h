#pragma once
#include "GameConfig.h"
#include "Sea_sign.h"
#include "Position.h"
#include <typeinfo>
#include <vector>
class Player;
class Enemy;
class Vortex;
class Octopus;


class Sea {
private:
    GameConfig config;
    int size_x;
    int size_y;
    Sea_sign **sea_field;

    void copyField(const Sea& other);

public:
    Sea(const GameConfig &cfg);
    Sea(const Sea& other) noexcept;
    Sea(Sea&& other) noexcept;
    Sea& operator=(const Sea& other);
    Sea& operator=(Sea&& other) noexcept;
    ~Sea();

    void allocateField();
    void deallocateField();
    int getWidth();
    int getHeight();
    bool isValidPosition(Position& pos);
    Sea_sign& getSeaZone(Position &pos);

    template <typename Type>
    bool SetOnSeaZone(Position &pos, Type &T){
        if(isValidPosition(pos)){
            if(typeid(Type) == typeid(Player)){
                sea_field[pos.y][pos.x].setFlag(player_flag);
                sea_field[pos.y][pos.x].setSign(player_flag);
                T.setPosition(pos);
            }else if(typeid(Type) == typeid(Enemy)){
                sea_field[pos.y][pos.x].setFlag(enemy_flag);
                sea_field[pos.y][pos.x].setSign(enemy_flag);
                T.setPosition(pos);
            }else if(typeid(Type) == typeid(Vortex)){
                sea_field[pos.y][pos.x].setFlag(vortex_flag);
                sea_field[pos.y][pos.x].setSign(vortex_flag);
                T.setPosition(pos);
            }else if(typeid(Type) == typeid(Octopus)){
                sea_field[pos.y][pos.x].setFlag(octopus_flag);
                sea_field[pos.y][pos.x].setSign(octopus_flag);
                T.setPosition(pos);
            }
            return true;
        }
        return false;
    }

    bool isEmptyPosition(Position& pos);
    void removeEntity(Position &pos);
    bool setTrapOnSeaZone(Position &pos);
    void display();
    std::vector<Position> emptyZones();

    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};
#pragma once
#include <memory>      
#include "characters/character.h" 
#include "characters/enemy.h"     

class EnemyHut : public Character{
    private:
        int moveCounter;
        int move;


    public:
        //Constructor/Destructor
        EnemyHut(int health, Position position, int counter = 5);

        //Active methods
        bool update();
        std::unique_ptr<Enemy> generateEnemy();


        // getters
        int getMove();
};      

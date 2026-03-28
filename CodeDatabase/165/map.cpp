#include "map.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <windows.h>
#include "./abstractNinterfaces/defines.h"
map::map(size_t height, size_t length, size_t difficultyLvL)
    : height(height), length(length), heroPtr(NULL), turn(1), difficultyLvL(difficultyLvL)
{
    if(height > MAXHEIGHT){
        height = MAXHEIGHT;
    }
    if(height < MINHEIGHT){
        height = MINHEIGHT;
    }
    if(length > MAXLENGTH){
        length = MAXLENGTH;
    }
    if(length < MINLENGTH){
        length = MINLENGTH;
    }
    this->tiles = new tile*[height];
    for(int i = 0; i < height; i++){
        this->tiles[i] = new tile[length];
        for(int j = 0; j < length; j++){
            this->tiles[i][j].name = NOTHING;
            this->tiles[i][j].address = NULL;
        }
    }
}
map::map(size_t height, size_t length) : map(height, length, 1){}

map::map() : map(MINHEIGHT, MINLENGTH){}

void map::generate(size_t enemyNum, size_t terrain, size_t castleNum, size_t trap)
{
    enemyNum = enemyNum*(0.9+MODIFICATOR*this->difficultyLvL) + difficultyLvL;
    castleNum = castleNum*(0.9+MODIFICATOR*this->difficultyLvL) + 1;
    terrain = terrain*(0.9+MODIFICATOR*this->difficultyLvL) + difficultyLvL;
    trap = trap*(0.9+MODIFICATOR*this->difficultyLvL) + 1;
    while(enemyNum + terrain + castleNum + trap >= this->height*this->length){
        srand(time(NULL));
        Sleep(1000);
        char random;
        random = rand()%3;
        switch(random){
            case 0:
                switch(rand()%2){
                    case 0:
                        if(enemyNum){
                            enemyNum--;
                        }
                        break;
                    case 1:
                        if(castleNum){
                            castleNum--;
                        }
                }
           case 1:
                if(terrain){
                    terrain--;
                }
            case 2:
                if(trap){
                    trap--;
                }
        }
    }
    int height_r, length_r; //its gonna be a long jorney
    srand(time(NULL));
    for(int i = 0; i < castleNum; i++){
        height_r = rand()%height;
        length_r = rand()%length;
        if(this->tiles[height_r][length_r].name == NOTHING){
            this->tiles[height_r][length_r].name = CASTLE;
            this->castles.push_back(new castle());
            this->castles[i]->setMapPosition(length_r, height_r);
            this->castles[i]->setId(i);
            this->tiles[height_r][length_r].address = this->castles[i];
        }else{
            i--;
        }
    }
    for(int i = 0; i < enemyNum; i++){
        height_r = rand()%height;
        length_r = rand()%length;
        if(this->tiles[height_r][length_r].name == NOTHING){
            this->tiles[height_r][length_r].name = ENEMY;
            this->enemies.push_back(new enemy());
            this->enemies[i]->setMapPosition(length_r, height_r);
            this->enemies[i]->setId(i);
            this->tiles[height_r][length_r].address = this->enemies[i];
        }else{
            i--;
        }
        
    }
    for(int i = 0; i < terrain; i++){
        height_r = rand()%height;
        length_r = rand()%length;
        if(this->tiles[height_r][length_r].name == NOTHING){
            this->tiles[height_r][length_r].name = BLOCK;
        }else{
            i--;
        }
    }
    for(int i = 0; i < trap; i++){
        height_r = rand()%height;
        length_r = rand()%length;
        if(this->tiles[height_r][length_r].name == NOTHING){
            this->tiles[height_r][length_r].name = TRAP;
        }else{
            i--;
        }
    }
    for(int i = 0; i < 1; i++){
        height_r = rand()%height;
        length_r = rand()%length;
        if(this->tiles[height_r][length_r].name == NOTHING){
            this->tiles[height_r][length_r].name = HERO;
            this->heroPtr = new hero();
            this->heroPtr->setMapPosition(length_r, height_r);
            this->tiles[height_r][length_r].address = this->heroPtr;
        }else{
            i--;
        }
    }
    std::cout << "You've spawned at (" << length_r << ", " << height_r << ")\nDifficulty: " << this->difficultyLvL << '\n';
}

void map::generate(size_t enemyNum, size_t terrain, size_t castlesNum)
{
    generate(enemyNum, terrain, castlesNum, 0);
}

void map::generate(size_t enemyNum, size_t terrain)
{
    generate(enemyNum, terrain, 0);
}

void map::generate(size_t enemyNum)
{
    generate(enemyNum, 0, 0);
}

void map::generate(){
    generate(3, 0, 0);
}

map::~map()
{
    for(enemy* x : this->enemies){
        delete x;
    }
    for(castle* x : this->castles){
        delete x;
    }
    for(int i = 0; i < height; i++){
        delete[] tiles[i];
    }
    delete this->heroPtr;
    delete[] tiles;
}

void map::show()
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < length; j++){
            std::cout << this->tiles[i][j].name << " \t";
        }
        std::cout << "\n\n";
    }
}

size_t map::getHeight()
{
    return this->height;
}

size_t map::getLength()
{
    return this->length;
}

tile **map::getTiles()
{
    return this->tiles;
}

hero *map::getHero()
{
    return this->heroPtr;
}

void map::setHero(hero * newHero)
{
    this->heroPtr = newHero;
}

std::vector<enemy *>& map::getEnemies()
{
    return this->enemies;
}

std::vector<castle *>& map::getCastles()
{
    return this->castles;
}

size_t map::getTurn()
{
    return this->turn;
}

void map::nextTurn()
{
    this->turn++;
}

void map::setTurn(size_t newTurn)
{
    this->turn = newTurn;
}

size_t map::getDifficultyLvL()
{
    return this->difficultyLvL;
}

void map::setDifficultyLvl(size_t newDifficultyLvL)
{
    this->difficultyLvL = newDifficultyLvL;
}

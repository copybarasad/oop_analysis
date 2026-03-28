#include "Game.h"

Game::Game(){
    //создаем gmanager с первым уровнем и заданным размером руки
    gmanager = GManager(GAME::START_HEIGHT, GAME::START_WIDTH, GAME::HAND_SIZE, GAME::FIRST_LEVEL);
}

void Game::process(){
    int level = GAME::FIRST_LEVEL;
    int height = GAME::START_HEIGHT;
    int width = GAME::START_WIDTH;
    
    gmanager.start_work();
    while (level < GAME::END_LEVEL){
        //gmanager.work() возвращает выполнено ли условие прохождения уровня
        //если оно осталось невыполненным и gmanager.work() закончил работу 
        //то это значит что игрок умер и мы заканчиваем игру
        if(!gmanager.work()) break;

        //создаем новый gmanager со следующим уровнем и сброшенными заклинаниями
        level++; height+=GAME::FIELD_CHANGE; width+=GAME::FIELD_CHANGE;
        gmanager = GManager(height, width, gmanager.get_reset_spells(), level);
        gmanager.start_work();
    }

    //мы прошли игру если мы прошли все уровни
    gmanager.end_work(level==GAME::END_LEVEL);
    return;
}

Game::~Game() {}

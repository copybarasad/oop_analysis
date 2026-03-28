#include "Render.h"
#include <iostream>
#include <iomanip>
using namespace std;


void ConsoleRenderer::renderMenu(){
    cout << setw(48) << right << "ДОБРО ПОЖАЛОВАТЬ В " << endl
    << setw(44) << right << "\033[1m\033[34mSUBNATICS\033[0m\033[0m" << endl << endl;

    cout << "n -------------- \033[44mНОВАЯ ИГРА\033[0m ------------- n" << endl;
    cout << "l -------------- \033[44mСОХРАНЕНИЯ\033[0m ------------- l" << endl;
    cout << "c ---------------- \033[44mУРОВЕНЬ\033[0m -------------- c" << endl;
    cout << "q ----------------- \033[44mВЫХОД\033[0m --------------- q" << endl;
}

void ConsoleRenderer::renderControls() {
    cout << "Управление:" << endl;
    cout << "WASD - движение" << endl;
    cout << "T - переключить тип атаки" << endl;
    cout << "R - дальняя атака (укажите координаты)" << endl;
    cout << "E - открыть сумку с заклинаниями" << endl;
    cout << "Q - выход" << endl;
    cout << "---------------------------------------------" << endl;
}

void ConsoleRenderer::renderStatistics(Game& game){
    cout << "------------ Игровая статистика ------------" << endl;
    game.getPlayer().getInfo();
    cout << setw(38) << left << "Врагов убито:" << setw(30) << left << game.getDefEns() << endl;
    cout << setw(41) << left << "Можно атаковать:" << setw(30) << left << (game.getPlayer().canAttackThisTurn() ? "Да" : "Нет") << endl;
    cout << "---------------------------------------------" << endl;
}

void ConsoleRenderer::renderField(Game& game){
    Sea field = game.getSea();

    cout << setw(4) << left << ' ';
    for(int j = 0; j < field.getWidth(); j++) {
        cout << setw(2) << left << j;
    }
    cout << endl;
    for(int y = 0; y < field.getHeight(); y++) {
        cout << setw(3) << y << '|';
        for(int x = 0; x < field.getWidth(); x++) {
            Position pos(x, y);
            if(field.getSeaZone(pos).getFlag() == player_flag){
                cout << "\033[34m" << field.getSeaZone(pos).getSign() << "\033[0m" << " ";
            }else if(field.getSeaZone(pos).getFlag() == enemy_flag || field.getSeaZone(pos).getFlag() == octopus_flag){
                cout << "\033[31m" << field.getSeaZone(pos).getSign() << "\033[0m" << " ";
            }else if(field.getSeaZone(pos).getFlag() == vortex_flag){
                cout << "\033[32m" << field.getSeaZone(pos).getSign() << "\033[0m" << " ";
            }else if(field.getSeaZone(pos).getFlag() == fish_net_flag){
                cout << "\033[33m" << field.getSeaZone(pos).getSign() << "\033[0m" << " ";
            }else{
                cout << field.getSeaZone(pos).getSign() << " ";
            }
        }
        cout << '|' << endl;
    }
}

void ConsoleRenderer::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}
#include "Control.h"
#include "Parser.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <optional>
using namespace std;


Control::Control(Game &tmp) : game(tmp) {}

void Control::executeCommand(const Command& cmd) {
    switch (cmd.id) {
        case CommandID::Move:
            game.plMovement(cmd.dx, cmd.dy);
            break;
        case CommandID::SwitchAttackType:
            game.plSwitchAttackType();
            break;
        case CommandID::RangedAttack:
            game.plRangedAttack(cmd.x, cmd.y);
            break;
        case CommandID::UseSpell:
            game.useSpell(cmd.spellIndex, cmd.x, cmd.y);
            break;
        case CommandID::Save: {
            try {
                game.save(cmd.path);
                cout << "\033[34mИгра сохранена.\033[0m" << endl;
            } catch (const exception& e) {
                cout << "\033[41mНе удалось сохранить: " << e.what() << "\033[0m" << endl;
            }
            break;
        }
        case CommandID::Load: {
            game.save("tmp_save.dat");
            try {
                game.load(cmd.path);
                cout << "\033[34mИгра загружена.\033[0m" << endl;
            } catch (const exception& e) {
                cout << "\033[41mНе удалось загрузить: " << e.what() << "\033[0m" << endl;
                try {
                    game.load("tmp_save.dat");
                } catch (...) {}
            }
            break;
        }
        case CommandID::Quit:
            break;
        case CommandID::None:
            break;
    }
}

int Control::cicle(){
    int freezeTime = 0;
    CommandParser parser;

    while(game.getIsGameOn()){
        game.printStatistics();
        game.printControls();
        game.render();

        auto cmdOpt = parser.parseInput();
        if (!cmdOpt) {
            continue;
        }

        const Command& cmd = *cmdOpt;
        executeCommand(cmd);

        bool isPlayerAction =
            cmd.id == CommandID::Move ||
            cmd.id == CommandID::RangedAttack ||
            cmd.id == CommandID::UseSpell;

        if(cmd.id == CommandID::Quit){
            return 0;
        }

        if (isPlayerAction) {
            if (freezeTime == 0) {
                game.octopus_attack();
                freezeTime = 2;
            }
            freezeTime--;
            game.enMovement();
            game.resetPlayerTurn();
        }

        cout << endl;
    }

    if(game.getIsWin()){
        cout << setw(37) << right << "\033[44mYou are win!\033[0m" << endl;
    }else{
        cout << setw(37) << right << "\033[41mGame is over\033[0m" << endl;
    }
    game.printStatistics();

    return 0;
}

int Control::start(){
    cout << setw(48) << right <<"ДОБРО ПОЖАЛОВАТЬ В " << endl
         << setw(44) << right << "\033[1m\033[34mSUBNATICS\033[0m\033[0m" << endl << endl;

    while(1){
        cout << "n -------------- \033[44mНОВАЯ ИГРА\033[0m ------------- n" << endl;
        cout << "l -------------- \033[44mСОХРАНЕНИЯ\033[0m ------------- l" << endl;
        cout << "c ---------------- \033[44mУРОВЕНЬ\033[0m -------------- c" << endl;
        cout << "q ----------------- \033[44mВЫХОД\033[0m --------------- q" << endl;
        
        char choise;
        do{
            cout << "\033[34mВыберите действие:\033[0m ";
            cin >> choise;
        }while(!startCheck(choise));

        switch(choise){
            case 'n':
                game.reset(game.getConfig());
                level_cicle(1, true);
                break;
            case 'l': {
                if(load("save.dat")){
                    level_cicle(game.getConfig().lvl_index, false);
                } else {
                    continue;
                }
                break;
            }
            case 'c':
                levels();
                break;
            case 'q':
                return 0;
            default:
                cout << "\033[41mНеизвестная команда\033[0m" << endl;
                break;
        }
        game.reset(game.getConfig());
    }
    return 0;
}

int Control::level_cicle(int cur_lvl, bool turn){
    if(turn){
        GameConfig new_cfg = lev.getLevel(cur_lvl);
        game.reset(new_cfg);
    }
    LevelConfig new_lvl_cfg = lev.getLevel(cur_lvl);
    cicle();
    

    if(new_lvl_cfg.next_lvl_ind && game.getIsWin()){
        cout << "\033[34mЖелаете перейти на следующий уровень?:\033[0m ";
        char choise;
        do{
            cout << "\033[34mВыберите действие (Y|N):\033[0m ";
            cin >> choise;
        }while(choise != 'Y' && choise != 'N');

        switch(choise){
            case 'Y':
                if(upgrade() == 1){
                    level_cicle(cur_lvl + 1, true);
                    break;
                }
                level_cicle(cur_lvl + 1, true);
                break;
            case 'N':
                return 0;
        }
    }else if(!new_lvl_cfg.next_lvl_ind && game.getIsWin()){
        cout << "ПОЗДРАВЛЯЕМ!" << endl << "игра пройдена" << endl;
    }
    return 0;
}

int Control::levels(){
    cout << "\033[44m-------- УРОВНИ ДЛЯ ПРОХОЖДЕНИЯ --------\033[0m" << endl << endl;
    for(int i = 1; i < 4; i++){
        cout << "\033[33m================================\033[0m" << endl;
        cout << i << "\033[34m: " << "уровень <<" << lev.getLevel(i).name << ">>\033[0m" << endl;
        cout << "\033[33m================================\033[0m" << endl;
    }

    int choise;
    do{
        cout << "Выберите уровень: " << endl;
        cin >> choise;
    }while(!checkInput());
    
    if(choise >= 1 && choise <= 3){
        level_cicle(choise, true);
    }
    
    return 0;
}

int Control::upgrade(){
    cout << "\033[44mВам доступны улучшении персонажа! Желаете прокачаться?\033[0m" << endl;
    UpgradeManager new_ups(3);
    ups = new_ups;
    
    while(ups.getSize() > 0){
        ups.display();
        char choise;
        do{
            cout << "\033[34mВыберите действие (Y|N):\033[0m ";
            cin >> choise;
        }while(choise != 'Y' && choise != 'N');

        switch (choise){
        case 'Y':
            int c;
            do{
                cout << "Выберите улучшение: " << endl;
                cin >> c;
            }while(!checkInput() || !upCheck(c, ups.getSize()));

            if(!useUpgrade(ups, c)){
                cout << "\033[41mОшибка!\033[0m" << endl;
                break;
            }
            break;
        case 'N':
            return 0;
        }
    }
    ups.reset();
    return 0;
}

bool Control::useUpgrade(UpgradeManager &ups, int c){
    Upgrade *up = ups.getUp(c);
    if(up == nullptr){
        cout << "\033[41mУлучшение вне диапозона\033[0m" << endl;
        return false;
    }
    
    if(up->getId() == 0){
        game.getPlayer().setDamage(ups.use(up, game.getPlayer().getDamage()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    }
    else if(up->getId() == 1){
        game.getPlayer().setHp(ups.use(up, game.getPlayer().getHp()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    }
    else if(up->getId() == 2){
        game.getPlayer().getBag().setMaxSize(ups.use(up, game.getPlayer().getBag().getMaxSize()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    }
    return true;
}

bool Control::upCheck(int choise, int size){
    if(choise < 0 || choise >= size){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool Control::startCheck(char choise){
    string funcs = "nlqc";
    if(funcs.find(choise) == string::npos){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool Control::checkInput(){
    if(cin.fail()){
        cout << "\033[41mОшибка ввода значений!\033[0m" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool Control::save(const string &path) const {
    try {
        game.save(path);
        cout << "\033[34mИгра сохранена.\033[0m" << endl;
        return true;
    } catch (const exception& e) {
        cout << "\033[41mНе удалось сохранить: " << e.what() << "\033[0m" << endl;
        return false;
    }
}

bool Control::load(const string &path) {
    save("tmp_save.dat");
    try {
        game.load(path);
        cout << "\033[34mИгра загружена.\033[0m" << endl;
        return true;
    } catch (const exception& e) {
        cout << "\033[41mНе удалось загрузить: " << e.what() << "\033[0m" << endl;
        game.load("tmp_save.dat");
        return false;
    }
}
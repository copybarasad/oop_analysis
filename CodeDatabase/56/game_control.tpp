#ifndef GAME_CONTROL_TPP
#define GAME_CONTROL_TPP

#include "game_control.h"
#include <iostream>
#include <sstream>
#include "spawn_system.h"
#include "movement_system.h"
#include "AIEnemy.h"
#include "spell_cast.h"
#include "random.h"

using namespace std;

template<typename InputType, typename RendererType>
GameControl<InputType, RendererType>::GameControl(Game* G)
    : game(G),
      renderer(std::make_unique<RendererType>()) {
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::renderMessage(const std::string& message) {
    if (renderer) {
        renderer->renderMessage(message);
    } else {
        cout << message << endl;
    }
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::clearScreen() {
    if (renderer) {
        renderer->clearScreen();
    }
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::enemy_move() {
    game->enemy_move();
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::building_move() {
    game->building_move();
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::spell_tower_move() {
    game->spell_tower_move();
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::newField(bool flag){
    game->newField(flag);
}

template<typename InputType, typename RendererType>
void GameControl<InputType, RendererType>::spellUp(){
    renderMessage("Выберите номер заклинания для улучшения: ");
    string spellsText = "Заклинания: ";
    auto spells = game->getPlayerSpells();
    for (size_t i = 0; i < spells.size(); ++i) {
        spellsText += to_string(i+1) + ". " + spells[i] + " ";
    }
    renderMessage(spellsText);
    
    string str;
    getline(cin, str);

    stringstream ss(str);
    string namesp;
    ss >> namesp;
    
    int numer;
    try {
        numer = stoi(namesp);
    } catch (const invalid_argument& e) {
        renderMessage("Ошибка: введены не числа!");
        return;
    }
    game->spellUp(numer);          
}

template<typename InputType, typename RendererType>
bool GameControl<InputType, RendererType>::game_on() {
    bool flag = false;
    while (!flag) {
        while (!game->isGameOver()) {
            if (renderer) {
                renderer->clearScreen();
                
                if (game->getField()) {
                    game->getField()->print_field();
                }
                
                auto spells = game->getPlayerSpells();
                renderer->renderGameState(
                    game->getPlayerHP(),
                    game->getPlayerMaxHP(),
                    game->getPlayerMana(),
                    game->getPlayerPoints(),
                    game->getEnemyCount(),
                    game->getBuildingCount(),
                    game->getCurrentTurn(),
                    game->getLevel(),
                    spells
                );
            }
            
            cout << "Команды: w/s/a/d/q/e/z/x/c (движение), f (атака), cast, melee/ranged, save, load, skip: ";
            
            Command cmd = input.getCommand();
            string move = cmd.action;

            if (move == "save") {
                if (cmd.filename.has_value()) {
                    if (game->saveGame(cmd.filename.value())) {
                        renderMessage("Игра сохранена!");
                    } else {
                        renderMessage("Ошибка сохранения игры!");
                    }
                } else {
                    string filename;
                    cout << "Введите имя сохранения: ";
                    getline(cin, filename);
                    if (game->saveGame(filename)) {
                        renderMessage("Игра сохранена!");
                    } else {
                        renderMessage("Ошибка сохранения игры!");
                    }
                }
                continue; 
            }
            else if (move == "load") {
                if (cmd.filename.has_value()) {
                    auto loadedGame = Game::loadGame(cmd.filename.value());
                    if (loadedGame) {
                        game = std::move(loadedGame);
                        renderMessage("Игра загружена!");
                    } else {
                        renderMessage("Ошибка загрузки игры!");
                    }
                } else {
                    string filename;
                    cout << "Введите имя сохранения для загрузки: ";
                    getline(cin, filename);
                    auto loadedGame = Game::loadGame(filename);
                    if (loadedGame) {
                        game = std::move(loadedGame);
                        renderMessage("Игра загружена!");
                    } else {
                        renderMessage("Ошибка загрузки игры!");
                    }
                }
                continue; 
            }
            else if (move == "skip") {
                renderMessage("Пропускаем ход...");
                continue;
            }
            else if (move == "move") {
                if (cmd.coordinates.has_value()) {
                    auto [dx, dy] = cmd.coordinates.value();
                    
                    Player* player = game->getPlayer();
                    if (!player) {
                        renderMessage("ОШИБКА: Игрок не найден!");
                        continue;
                    }

                    if (!game->movePlayer(dx, dy)) {
                        renderMessage("Не могу двигаться туда! Возможно препятствие.");
                    }
                } else {
                    renderMessage("Введите куда ходите (x y от -1 до 1):");
                    int x_move, y_move;
                    
                    string line;
                    getline(cin, line);
                    stringstream ss(line);

                    string str1, str2;
                    ss >> str1 >> str2;
                    
                    if (str1.empty() || str2.empty()) {
                        renderMessage("ОШИБКА: Нужно ввести два числа!");
                        continue;
                    }
                    
                    try {
                        x_move = stoi(str1);
                        y_move = stoi(str2);
                    } catch (...) {
                        renderMessage("ОШИБКА: Введены не числа!");
                        continue;
                    }
                    
                    if (x_move < -1 || x_move > 1 || y_move < -1 || y_move > 1) {
                        renderMessage("ОШИБКА: Координаты должны быть от -1 до 1!");
                        continue;
                    }
                    
                    if (!game->movePlayer(x_move, y_move)){
                        renderMessage("Не могу двигаться туда! Возможно препятствие.");
                        continue;
                    }
                }
            }
            else if (move == "attack") {
                if (cmd.coordinates.has_value()) {
                    auto [dx, dy] = cmd.coordinates.value();
                    
                    Player* player = game->getPlayer();
                    if (!player) {
                        renderMessage("ОШИБКА: Игрок не найден!");
                        continue;
                    }
                    

                    if (!game->attack(dx, dy)) {
                        renderMessage("Не могу атаковать туда! Нет цели в этом направлении.");
                    } else {
                        renderMessage("Атака успешна!");
                    }
                } else {
                    renderMessage("Введите направление атаки (x y от -1 до 1):");
                    
                    string line;
                    getline(cin, line);
                    stringstream ss(line);

                    string str1, str2;
                    ss >> str1 >> str2;
    
                    if (str1.empty() || str2.empty()) {
                        renderMessage("ОШИБКА: Нужно ввести два числа!");
                        continue;
                    }
    
                    int x_move, y_move;
                    try {
                        x_move = stoi(str1);
                        y_move = stoi(str2);
                    } catch (...) {
                        renderMessage("ОШИБКА: Введены не числа!");
                        continue;
                    }
                    
                    if (x_move < -1 || x_move > 1 || y_move < -1 || y_move > 1) {
                        renderMessage("ОШИБКА: Координаты должны быть от -1 до 1!");
                        continue;
                    }
    
                    if (!game->attack(x_move, y_move)) {
                        renderMessage("Неправильная атака! Нет цели в этом направлении.");
                        continue;
                    } else {
                        renderMessage("Атака успешна!");
                    }
                }
            }
            else if (move == "help") {
                renderMessage("=== Помощь ===");
                renderMessage("Атака: f (затем выберите направление)");
                renderMessage("Смена типа: melee/ranged");
                renderMessage("Заклинание: cast");
                renderMessage("Сохранение/загрузка: save/load");
                renderMessage("Пропуск хода: skip");
                renderMessage("Помощь: help");
                continue;
            }
            else if (move == "melee") {
                game->switchToMelee();
                renderMessage("Переключено на ближний бой");
            }
            else if (move == "ranged") {
                game->switchToRanged();
                renderMessage("Переключено на дальний бой");
            }
            else if (move == "cast") {
                string spell_use;
                if (cmd.spell_name.has_value()) {
                    spell_use = cmd.spell_name.value();
                } else {
                    renderMessage("Введите название заклинания которое хотите использовать: ");
                    getline(cin, spell_use);
                }
                
                if (!game->searchSpellPlayer(spell_use)) {
                    renderMessage("У вас нет такого заклинания.");
                    continue;
                }
                
                pair<int, int> coords;
                if (cmd.coordinates.has_value()) {
                    coords = cmd.coordinates.value();
                } else {
                    renderMessage("Введите координаты куда хотите произвести удар (x y): ");
                    string coord_line;
                    getline(cin, coord_line);
                    stringstream coord_ss(coord_line);
                    coord_ss >> coords.first >> coords.second;
                }

                if (!game->useSpell(spell_use, coords.first, coords.second)){
                    renderMessage("Не удалось использовать заклинание");
                    continue;
                }

                game->DelSpell(spell_use);
                renderMessage("Заклинание " + spell_use + " использовано!");
            }
            else if (move == "move_input" || move == "attack_input") {
                string actual_move = (move == "move_input") ? "move" : "attack";
                if (player_move(actual_move) == false) {
                    renderMessage("Неверная команда");
                    continue;
                }
            }
            else {
                if (player_move(move) == false) {
                    renderMessage("Неверная команда");
                    continue;
                }
            }
    
            if (game->getHpPlayer() <= 0) {
                renderMessage("Игра окончена! Вы проиграли.");
                break;
            }

            enemy_move();
            building_move();
            spell_tower_move();
        
            if (game->getCntEnemyWithGame() == 0 && game->getCntBuildingWithGame() == 0) {
                renderMessage("Поздравляем! Вы победили!");
                flag = true;
                break;
            }
    
            game->incrementTurn();
        }
    
        if (!flag) {
            renderMessage("Хотите ли вы начать игру заново? да/нет");
            string strin;
            getline(cin, strin);
            stringstream ss(strin);
            string s;
            ss >> s;
            if (s == "да") {
                newField(false);
            } else {
                flag = true; 
            }
        } else {
            renderMessage("Переход на новый уровень");
            newField(true);
            game->levelUpPlayer();
            game->restorePlayerHP();
            renderMessage("Игрок получил улучшение!"); 
            
            spellUp();
            flag = false;
        }
    }
    return true;
}

template<typename InputType, typename RendererType>
bool GameControl<InputType, RendererType>::player_move(const string& move) {
    if (game->getHpPlayer() <= 0) {
        return true;
    }
    
    if (move == "melee") {
        game->switchToMelee();
        return true;
    } 
    else if (move == "ranged") {
        game->switchToRanged();
        return true;
    }
    else if (move == "move") {
        cout << "Введите куда ходите (x y от -1 до 1):\n";
        int x_move, y_move;
        
        string line;
        getline(cin, line);
        if (line.empty()) {
            cout << "ОШИБКА: Пустой ввод!\n";
            return false;
        }
        
        stringstream ss(line);
        string str1, str2;
        ss >> str1 >> str2;
        
        if (str1.empty() || str2.empty()) {
            cout << "ОШИБКА: Нужно ввести два числа!\n";
            return false;
        }
        
        try {
            x_move = stoi(str1);
            y_move = stoi(str2);
        } catch (...) {
            cout << "ОШИБКА: Введены не числа!\n";
            return false;
        }
        
        if (x_move < -1 || x_move > 1 || y_move < -1 || y_move > 1) {
            cout << "ОШИБКА: Координаты должны быть от -1 до 1!\n";
            return false;
        }
        
        return game->movePlayer(x_move, y_move);
    }
    else if (move == "attack") {
        cout << "Введите направление атаки (x y от -1 до 1):\n ";
    
        string line;
        getline(cin, line);
        if (line.empty()) {
            cout << "ОШИБКА: Пустой ввод!\n";
            return false;
        }
        
        stringstream ss(line);
        string str1, str2;
        ss >> str1 >> str2;
    
        if (str1.empty() || str2.empty()) {
            cout << "ОШИБКА: Нужно ввести два числа!\n";
            return false;
        }
    
        int x_move, y_move;
        try {
            x_move = stoi(str1);
            y_move = stoi(str2);
        } catch (...) {
            cout << "ОШИБКА: Введены не числа!\n";
            return false;
        }
        
        if (x_move < -1 || x_move > 1 || y_move < -1 || y_move > 1) {
            cout << "ОШИБКА: Координаты должны быть от -1 до 1!\n";
            return false;
        }
    
        return game->attack(x_move, y_move);
    }
    else if (move == "cast") {
        string spell_use;
        cout << "Введите название заклинания которое хотите использовать: ";
        getline(cin, spell_use); 
    
        if (!game->searchSpellPlayer(spell_use)) {
            cout << "У вас нет такого заклинания.\n";
            return false;
        }
    
        cout << "Введите координаты куда хотите произвести удар: ";
        string coord_line;
        getline(cin, coord_line);
    
        stringstream coord_ss(coord_line);
        int x_move, y_move;
        coord_ss >> x_move >> y_move;

        if (!game->useSpell(spell_use, x_move, y_move)){
            cout << "Не удалось использовать заклинание\n";
            return false;
        }

        game->DelSpell(spell_use);
        return true;
    }
    else {
        cout << "Неизвестная команда: " << move << "\n";
        return false;
    }
}

#endif
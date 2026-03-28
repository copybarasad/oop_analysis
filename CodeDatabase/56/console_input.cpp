#include "console_input.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

optional<pair<int, int>> ConsoleInput::directionToCoordinates(const string& direction) {
    if (direction == "s") return make_pair(0, 1);
    else if (direction == "w") return make_pair(0, -1);
    else if (direction == "e") return make_pair(1, -1);
    else if (direction == "q") return make_pair(-1, -1);
    else if (direction == "a") return make_pair(-1, 0);
    else if (direction == "z") return make_pair(-1, 1);
    else if (direction == "x") return make_pair(0, 1);
    else if (direction == "c") return make_pair(1, 1);
    else if (direction == "d") return make_pair(1, 0);
        
    cout << "Неизвестное направление: " << direction << endl;
    return nullopt;
}

Command ConsoleInput::getCommand() {
    Command command;
    string input;
    
    getline(cin, input);
    stringstream ss(input);
    string move;
    ss >> move;

    if (move == "o" || move == "l") {
        command.action = move;
        string filename;
        cout << "Введите название сохранения: ";
        getline(cin, filename);
        command.filename = filename;
    } 
    else if (move == "s" || move == "w" || move == "e" || move == "q" || 
             move == "a" || move == "z" || move == "x" || move == "c" || move == "d") {
        command.action = "move";
        command.coordinates = directionToCoordinates(move);
    } 
    else if (move == "g") {
        command.action = "attack";
        cout << "Введите координаты куда хотите произвести удар: ";
            
        string line;
        getline(cin, line);
        stringstream coord_ss(line);

        string str1, str2;
        coord_ss >> str1 >> str2;

        if (str1.empty() || str2.empty()) {
            cout << "Ошибка: нужно ввести два числа!\n";
            command.action = "ERROR";
        } else {
            try {
                int x_move = stoi(str1);
                int y_move = stoi(str2);
                command.coordinates = make_pair(x_move, y_move);
            } catch (const invalid_argument& e) {
                cout << "Ошибка: введены не числа!\n";
                command.action = "ERROR";
            } catch (const out_of_range& e) {
                cout << "Ошибка: числа слишком большие!\n";
                command.action = "ERROR";
            }
        }
    } 
    else if (move == "h") {
        command.action = "help";
    } 
    else if (move == "f") {
        command.action = "attack";
    
        cout << "Введите направление атаки (s/w/e/q/a/z/x/c/d): ";
        string direction;
        getline(cin, direction);
        
        string firstWord;
        stringstream ss(direction);
        ss >> firstWord;
        command.coordinates = directionToCoordinates(firstWord);
    }
    else if (move == "save") {
        command.action = "save";
    }
    else if (move == "load") {
        command.action = "load";
    }
    else if (move == "skip") {
        command.action = "skip";
    }
    else if (move == "melee") {
        command.action = "melee";
    }
    else if (move == "ranged") {
        command.action = "ranged";
    }
    else if (move == "cast") {
        command.action = "cast";
    }
    else if (move == "move" || move == "m") {
        command.action = "move_input";
    }
    else if (move == "attack" || move == "a") {
        command.action = "attack_input";
    }
    else {
        command.action = move;
    }
    
    return command;
}


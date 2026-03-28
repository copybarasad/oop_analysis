#pragma once
#include <iostream>
#include <string>
#include "command.h"

class ConsoleInputReader {
public:
    Command read() {
        Command cmd;
        std::cout << "\n=== ’од игрока ===\n";
        std::cout << "1 - ход\n2 - атака\n3 - заклинание\n4 - показать заклинани€\n5 - сохранить\n6 - загрузить\n¬аш выбор: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dump; std::getline(std::cin, dump);
            return cmd;
        }

        switch (choice) {
        case 1: {
            cmd.type = CommandType::Move;
            std::cout << "’од (W/A/S/D): ";
            std::cin >> cmd.dir;
            break;
        }
        case 2: {
            cmd.type = CommandType::Attack;
            std::cout << "¬ведите координаты атаки (r c): ";
            std::cin >> cmd.r >> cmd.c;
            break;
        }
        case 3: {
            cmd.type = CommandType::Spell;
            std::cout << "¬ведите номер заклинани€: ";
            std::cin >> cmd.spell_index;
            std::cout << "¬ведите координаты цели (r c): ";
            std::cin >> cmd.r >> cmd.c;
            break;
        }
        case 4:
            cmd.type = CommandType::ShowSpells;
            break;
        case 5:
            cmd.type = CommandType::Save;
            std::cout << "¬ведите им€ файла: ";
            std::cin >> cmd.filename;
            break;
        case 6:
            cmd.type = CommandType::Load;
            std::cout << "¬ведите им€ файла: ";
            std::cin >> cmd.filename;
            break;

        default:
            cmd.type = CommandType::None;
            break;
        }

        return cmd;
    }
};

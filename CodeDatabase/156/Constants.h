//
// Created by Artem on 07.10.2025.
//

#ifndef LABA1_CONSTANTS_H
#define LABA1_CONSTANTS_H


#include <vector>
#include <iostream>
#include "../Entities/Creatures/Attacks/Spels/SpellType.h"
#include "../Entities/EntityType.h"
#include <limits>
#include <sstream>
#include <conio.h>

class Constants {
public:
    struct XYPair{
        int x, y;
        bool operator==(const XYPair& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct dxdy{ //нужна чтобы в некоторый контекст можно было передать именно её
        int x, y;

        bool operator==(const dxdy& other) const {
            return x == other.x && y == other.y;
        }
    };
    static inline const std::vector<Constants::dxdy> dxdys{{-1, 1},
                                                      {0,  1},
                                                      {1,  1},
                                                      {1,  0},
                                                      {1,  -1},
                                                      {0,  -1},
                                                      {-1, -1},
                                                      {-1, 0}};

    static const std::string spellTypeToString(SpellType type) {
        switch (type) {
            case SpellType::DirectDamageSpell: return "DirectDamageSpell";
            case SpellType::AreaDamageSpell: return "AreaDamageSpell";
            case SpellType::SummoningSpell: return "SummoningSpell";
            case SpellType::BuffNextUsedSpell: return "BuffNextUsedSpell";
            case SpellType::CreateTrapSpell: return "CreateTrapSpell";
            default: return "UnknownSpellType";
        }
    }
    static const std::string entityTypeToString(EntityType type) {
        switch (type) {
            case EntityType::PlayerEnt:        return "PlayerEnt";
            case EntityType::EnemyEnt:         return "EnemyEnt";
            case EntityType::EnemySpawnerBuildingEnt: return "EnemySpawnerBuildingEnt";
            case EntityType::Ally:             return "Ally";
            case EntityType::EnemyDefenceTower: return "EnemyDefenceTower";
            default:                           return "UnknownEntityType";
        }
    }

    template<typename T>
    static T getInput(const std::string& prompt = "") {
        if (!prompt.empty()) {
            std::cout << prompt;
        }

        std::string line;
        while (true) {
            std::getline(std::cin, line);

            if constexpr (std::is_same_v<T, std::string>) {
                return line;
            } else {
                std::istringstream iss(line);
                T value;
                if (iss >> value && iss.eof()) {
                    return value;
                } else {
                    std::cout << "Что-то пошло не так! Введите правильное значение: " << std::endl;
                }
            }
        }
    }

    static char getInputWithoutN(const std::string& prompt = ""){
        if(!prompt.empty()){
            std::cout << prompt;
        }
        while (true) {
            char ch = _getch();
            while (_kbhit()) {
                _getch();
            }
            if (ch >= 32 && ch <= 126) {
                std::cout << ch << std::endl;
                return ch;
            }
            std::cout << "Что-то пошло не так! Введите нормальный символ: " << std::endl;
        }
    }

    static std::pair<int, int> readTwoInts(const std::string& prompt = ""){
        if(!prompt.empty()){
            std::cout << prompt;
        }

        std::pair<int, int> result;

        while (true) {
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            if (iss >> result.first >> result.second) {
                std::string remaining;
                if (iss >> remaining) {
                    std::cout << "Обнаружены лишние символы. Введите только два числа: ";
                    continue;
                }
                return result;
            }
            std::cout << "Неверный ввод. Введите два целых числа через пробел: ";
        }

    }

};


#endif //LABA1_CONSTANTS_H

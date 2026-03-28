#pragma once
#include <string>
#include <vector>

struct SerializableGameData
{
    // Значения поля
    int _fieldWidth;
    int _fieldHeight;
    std::vector<int> _walls;
    // Значения игрока
    std::vector<int> _position;
    int _speed;
    int _maxHealth;
    int _nearDamage;
    int _farDamage;
    // Значения противника
    std::vector<int> _position;
    int _speed;
    int _maxHealth;
    int _nearDamage;
    int _farDamage;

};

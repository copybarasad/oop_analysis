#include <iostream>
#include <sstream>
#include "../../Headers/Realizations/Enemy.h"
#include "../../Headers/Math.h"

Enemy::EnemyData::EnemyData(const Enemy* enemy) : _position{ enemy->_position },
    _speed{ enemy->_speed }, _maxHealth{ enemy->_maxHealth }, _damage{ enemy->_damage }
{
}

Enemy::EnemyData::EnemyData(std::string& s, int index)
{
    std::istringstream stream{ s };
    std::string line;
    while (line != "ENEMY_S" + std::to_string(index) && std::getline(stream, line))
    {
    }
    std::getline(stream, line);
    std::istringstream positionStream{ line };
    _position = Cell();
    positionStream >> _position._x >> _position._y;
    if (_position == Cell())
        return;
    std::getline(stream, line);
    _speed = std::stoi(line);
    std::getline(stream, line);
    _maxHealth = std::stoi(line);
    std::getline(stream, line);
    _damage = std::stoi(line);
}

std::string Enemy::EnemyData::ToString(int index) const
{
    std::string stringIndex = std::to_string(index);
    std::string s = "ENEMY_S" + stringIndex + '\n' + std::to_string(_position._x) + ' ' + std::to_string(_position._y) +
        '\n' + std::to_string(_speed) + '\n' + std::to_string(_maxHealth) + '\n' + std::to_string(_damage) +
        "\nENEMY_E" + stringIndex;
    return s;
}

Enemy::Enemy(const MortalContainer* container, Cell position, int speed, int maxHealth, int damage) :
    Movable(position, speed), _container{ container }, _health(maxHealth), _maxHealth(maxHealth), _damage(damage)
{
}

void Enemy::OnMoveSuccess(Cell target)
{
    std::cout << "Противник перешёл на точку (" << target._x << ";" << target._y << ")" << std::endl;
}

void Enemy::OnMoveFailure(MoveResult result)
{
    if (result._collision)
    {
        IMortal* player = _container->GetEntity(result._collision);
        if (player)
        {
            player->TakeDamage(_damage);
            std::cout << "Противник напал на игрока." << std::endl;
            return;
        }
        std::cout << "Противник не может атаковать неуязвимую цель." << std::endl;
    }
    std::cout << "Противник не может пройти дальше точки (" << result._destination._x << ";" <<
        result._destination._y << ")." << std::endl;
}

void Enemy::TakeDamage(int damage)
{
    _health = clamp(_health - damage, 0, 1000);
    std::cout << "Здоровье противника: " << _health << std::endl;
}

void Enemy::ChangeDamageByValue(int value)
{
    _damage = value;
}

int Enemy::GetHealth()
{
    return _health;
}

int Enemy::GetMaxHealth()
{
    return _maxHealth;
}

int Enemy::GetDamage()
{
    return _damage;
}

bool Enemy::IsAlive()
{
    return _health > 0;
}

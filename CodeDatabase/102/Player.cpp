#include <iostream>
#include <sstream>
#include "../../Headers/Realizations/Player.h"
#include "../../Headers/Math.h"

Player::PlayerData::PlayerData(const Player* player) : _position{ player->_position },
    _speed{ player->_speed }, _maxHealth{ player->_maxHealth }, _nearDamage{ player->_sourceNearDamage },
    _farDamage{ player->_sourceFarDamage }
{
}

Player::PlayerData::PlayerData(std::string& s)
{
    std::istringstream stream{ s };
    std::string line;
    while (line != "PLAYER_S" && std::getline(stream, line))
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
    _nearDamage = std::stoi(line);
    std::getline(stream, line);
    _farDamage = std::stoi(line);
}

std::string Player::PlayerData::ToString() const
{
    std::string s = "PLAYER_S\n" + std::to_string(_position._x) + ' ' + std::to_string(_position._y) + '\n' +
        std::to_string(_speed) + '\n' + std::to_string(_maxHealth) + '\n' + std::to_string(_nearDamage) +
            '\n' + std::to_string(_farDamage) + "\nPLAYER_E";
    return s;
}

Player::Player(const MortalContainer* container, Cell position, int speed, int maxHealth, int nearDamage,
               int farDamage) : Movable(position, speed), _container{container}, _health(maxHealth), _maxHealth(maxHealth),
                                _damage(nearDamage), _sourceNearDamage(nearDamage), _sourceFarDamage(farDamage)
{
}

void Player::OnMoveSuccess(Cell target)
{
    std::cout << "Игрок перемещён на точку (" << target._x << ";" << target._y << ")" << std::endl;
}

void Player::OnMoveFailure(MoveResult result)
{
    if (result._collision)
    {
        IMortal* enemy = _container->GetEntity(result._collision);
        if (enemy)
        {
            enemy->TakeDamage(_damage);
            std::cout << "Игрок напал на противника." << std::endl;
            return;
        }
        std::cout << "Игрок не может атаковать неуязвимую цель." << std::endl;
    }
    std::cout << "Игрок не может пройти дальше точки (" << result._destination._x << ";" << result._destination._y <<
        ")." << std::endl;
}

void Player::SwitchDamageMode(int index)
{
    if (index == 1)
    {
        _damage = _sourceFarDamage;
        std::cout << "Игрок включил режим дальнего боя (урон " << _damage << ")." << std::endl;
    }
    else if (index == 0)
    {
        _damage = _sourceNearDamage;
        std::cout << "Игрок включил режим ближнего боя (урон " << _damage << ")." << std::endl;
    }
    else
        return;
    _currentModeIndex = index;
}

void Player::ChangeDamageByValue(int value)
{
    _damage = value;
}

int Player::GetHealth()
{
    return _health;
}

int Player::GetMaxHealth()
{
    return _maxHealth;
}

int Player::GetCurrentModeIndex() const
{
    return _currentModeIndex;
}

int Player::GetDamage()
{
    return _damage;
}

void Player::TakeDamage(int damage)
{
    _health = clamp(_health - damage, 0, 1000);
    std::cout << "Здоровье игрока: " << _health << std::endl;
}

bool Player::IsAlive()
{
    return _health > 0;
}
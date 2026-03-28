#include "player.h"
#include <stdlib.h>
#include <ctime>


Player::Player(int x, int y, int size) {
    health = 100;
    damage = 5;
    type = close_combat;
    player_position = nullptr;
    count_for_slow = 0;
    zoneX = 100;
    zoneY = 100;
    money = 100;
    spawn_player(x, y);
    myHand = new handPlayer(size);
}
Player::~Player()
{
    delete player_position;
    delete myHand;
}

Point* Player::get_position() const
{
    return player_position;
}

int Player::get_x() const
{
    return this->player_position->get_point_x();
}

int Player::get_y() const
{
    return this->player_position->get_point_y();
}

Point& Player::set_position()
{
    return *player_position;
}


void Player::set_damage(int value)
{
    this->damage = value;
}

void Player::set_zoneX(int v)
{
    this->zoneX = v;
}

void Player::set_zoneY(int v)
{
    this->zoneY = v;
}

void Player::set_count_for_slow(int val)
{
    this->count_for_slow += val;
}


void Player::spawn_player(int x, int y)
{
    this->player_position = new Point(x, y, unlock);
}

void Player::player_journey(int dx, int dy)
{
    this->set_position().set_point_x(dx);
    this->set_position().set_point_y(dy);

}

void Player::change_typeBattle()
{
    if (this->type == close_combat){
        this->type = ranged_combat;
        this->set_damage(3);
    }
    else{
        this->type = close_combat;
        this->set_damage(5);
    }
}

TypeBattle Player::get_typeBattle() const
{
    return this->type;
}

Directions Player::get_direction() const
{
    return this->dir;
}


int Player::get_damage() const
{
    return this->damage;
}

int Player::get_zoneX() const
{
    return zoneX;
}

int Player::get_zoneY() const
{
    return zoneY;
}

int Player::get_money() const
{
    return money;
}

int Player::get_count_for_slow() const
{
    return count_for_slow;
}

handPlayer* Player::get_hand()
{
    return myHand;
}

void Player::change_direction_attack(Directions d)
{
    this->dir = d;
}

void Player::change_health(int n)
{
    if(n <= 0){
        health = 0;
        return;
    }
    health = n;

}

void Player::add_money(int val)
{
    this->money += val;
}

void Player::spend_money(int val)
{
    this->money -= val;
}

void Player::recovery_health()
{
    health = 100;
}

int Player::get_health() const
{
    return health;
}


std::vector<int> Player::close_attack()
{
    std::vector<int> info_attack;
    info_attack.push_back(this->get_x());
    info_attack.push_back(this->get_y());
    info_attack.push_back(this->damage);
    return info_attack;
}


std::vector<int> Player::ranged_atack()
{
    int user_x = this->get_x();
    int user_y = this->get_y();

    int d = this->get_direction();

    switch(d){
    case 0:
        this->set_zoneX(user_x - 1);
        this->set_zoneY(user_y - 7);
        break;
    case 1:

        this->set_zoneX(user_x - 1);
        this->set_zoneY(user_y + 5);
        break;
    case 2:

        this->set_zoneX(user_x + 5);
        this->set_zoneY(user_y - 1);
        break;
    case 3:

        this->set_zoneX(user_x - 7);
        this->set_zoneY(user_y - 1);
        break;
    }

    std::vector<int> info_attack;
    info_attack.push_back(this->get_x());
    info_attack.push_back(this->get_y());
    info_attack.push_back(this->damage);
    info_attack.push_back(this->dir);
    return info_attack;
}



void Player::serialize(std::ostream& os) const  {
    if (!os.good()) {
        throw std::runtime_error("Поток для записи в плохом состоянии");
    }

    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&score), sizeof(score));
    os.write(reinterpret_cast<const char*>(&count_for_slow), sizeof(count_for_slow));
    os.write(reinterpret_cast<const char*>(&zoneX), sizeof(zoneX));
    os.write(reinterpret_cast<const char*>(&zoneY), sizeof(zoneY));
    os.write(reinterpret_cast<const char*>(&money), sizeof(money));

    int typeValue = static_cast<int>(type);
    os.write(reinterpret_cast<const char*>(&typeValue), sizeof(typeValue));

    int dirValue = static_cast<int>(dir);
    os.write(reinterpret_cast<const char*>(&dirValue), sizeof(dirValue));

    if (player_position) {
        bool hasPosition = true;
        os.write(reinterpret_cast<const char*>(&hasPosition), sizeof(hasPosition));
        player_position->serialize(os);
    } else {
        bool hasPosition = false;
        os.write(reinterpret_cast<const char*>(&hasPosition), sizeof(hasPosition));
    }

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Player в поток");
    }
}

void Player::deserialize(std::istream& is) {
    if (!is.good()) {
        throw std::runtime_error("Поток для чтения в плохом состоянии");
    }

    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&score), sizeof(score));
    is.read(reinterpret_cast<char*>(&count_for_slow), sizeof(count_for_slow));
    is.read(reinterpret_cast<char*>(&zoneX), sizeof(zoneX));
    is.read(reinterpret_cast<char*>(&zoneY), sizeof(zoneY));
    is.read(reinterpret_cast<char*>(&money), sizeof(money));

    int typeValue, dirValue;
    is.read(reinterpret_cast<char*>(&typeValue), sizeof(typeValue));
    is.read(reinterpret_cast<char*>(&dirValue), sizeof(dirValue));

    if (typeValue < static_cast<int>(TypeBattle::close_combat) ||
        typeValue > static_cast<int>(TypeBattle::ranged_combat)) {
        throw std::runtime_error("Некорректное значение TypeBattle");
    }
    type = static_cast<TypeBattle>(typeValue);

    if (dirValue < static_cast<int>(Directions::up) ||
        dirValue > static_cast<int>(Directions::left)) {
        throw std::runtime_error("Некорректное значение Directions");
    }
    dir = static_cast<Directions>(dirValue);

    bool hasPosition;
    is.read(reinterpret_cast<char*>(&hasPosition), sizeof(hasPosition));
    if (hasPosition) {
        if (!player_position) {
            player_position = new Point();
        }
        player_position->deserialize(is);
    } else {
        delete player_position;
        player_position = nullptr;
    }

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Player из потока");
    }
}

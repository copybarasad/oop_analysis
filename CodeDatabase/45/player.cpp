#include "player.hpp"
#include <iostream>
#include <unistd.h>
#include "hand.hpp"

Player::Player(int health, int damage, int max_hp) 
    : Entity(health, damage), max_hp(max_hp){
        dtype = DamageType::MELEE;
}

void Player::setDamageType(DamageType newDamageType) {
    if (dtype == DamageType::MELEE && newDamageType == DamageType::RANGE) {
        damage /= 2;
    } else if (dtype == DamageType::RANGE && newDamageType == DamageType::MELEE) {
        damage *= 2;
    }
    dtype = newDamageType;
}


DamageType Player::getDamageType() {
    return dtype;
}

void Player::set_hand(Hand* hand_ptr) {
    hand = hand_ptr;
}

void Player::step(Map* map, int x, int y) {}

/*
void Player::step(Map* map, int x, int y) {
    int deltax=0,deltay=0,newx,newy;
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
            last_step = key;
            switch (key) {
                case 'w':
                case 'W':
                    deltay=-1;
                    break;
                    
                case 'a':
                case 'A':
                    deltax=-1;
                    break;
                    
                case 's':
                case 'S':
                    deltay=1;
                    break;
                    
                case 'd':
                case 'D':
                    deltax=1;
                    break;

                case 'm':
                case 'M':
                    setDamageType(DamageType::MELEE);
                    break;
                
                case 'r':
                case 'R':
                    setDamageType(DamageType::RANGE);
                    break;
                    
                default:
                    if (key >= '0' && key <= '9') {
                        hand->castSpell(key - '0', map);
                    } else {
                        //std::cout << "Нажата другая клавиша: " << key 
                            //<< " (код: " << static_cast<int>(key) << ")" << std::endl;
                    }
                    
                    break;
            }
            
            if (deltax+deltay != 0) {
                if (dtype == DamageType::MELEE) {
                    newx=x+deltax;
                    newy=y+deltay;
                    map->moveEntity(x, y, newx, newy);
                } else if (dtype == DamageType::RANGE) {
                    int nx=x+deltax;
                    int ny=y+deltay;

                    while (0 <= nx && nx < map->getWidth() && 0 <= ny && ny < map->getHeight()) {
                        if (map->getCellState(nx, ny) == CellState::MONSTER || map->getCellState(nx, ny) == CellState::WALL || map->getCellState(nx, ny) == CellState::TOWER) {
                            std::cout << "x=\n";
                            map->moveEntity(x, y, nx, ny);
                            break;
                        }
                        nx+=deltax;
                        ny+=deltay;
                    }
                }
                
            }

        }
}*/

char Player::get_last_step() {
    return last_step;
}

std::string Player::getStatus() const {
    return "Player," + std::to_string(hp) + "," + std::to_string(damage) + "," + std::to_string(max_hp) + "," + (dtype == DamageType::MELEE ? "M" : "R");
}

void Player::heal_hp() {
    hp = max_hp;
}

void Player::increase_hp() {
    max_hp+=50;
}

void Player::increase_damage() {
    if (dtype == DamageType::MELEE) {
        damage += 20;
    } else {
        damage += 10;
    }
}
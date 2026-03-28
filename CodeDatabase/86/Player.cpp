#include "Player.hpp"
#include <iostream>

Player::Player(int hp, int damage, Position start_pos, Logger& logger)
    : Character(hp, damage, start_pos, logger), sleeve_(3) {}

bool Player::move(Position pos, Grid& grid) {
    if(!isAlive()) return false;
    if(!grid.IsFreeCell(pos.x, pos.y)) return false;
    
    bool isSlowed = grid.slowedCell(pos.x, pos.y);
    setPos(pos);
    
    if(isSlowed) {
        logger_.warning("Player stepped on a slowed tile! Movement speed reduced.");
        logger_.action("Player moved to (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ") [SLOWED]");
    } else {
        logger_.action("Player moved to (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")");
    }
    
    return !isSlowed; 
}

void Player::attackEnemy(Character& enemy) {
    int dist = distanceTo(enemy);
    if(attack == attackType::melee && dist == 1) {
        logger_.action("Player attacks enemy (melee)!");
        melee_attack(enemy);
    } else if(attack == attackType::range && dist <= 6) {
        logger_.action("Player attacks enemy (ranged)!");
        range_attack(enemy);
    } else {
        logger_.warning("Enemy out of range!");
    }
}

bool Player::act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx) {
    switch(a) {
        case action::Move: {
            std::cout << "Move by (x y): ";
            int dx, dy;
            
            while(true) {
                std::cin >> dx >> dy;
                
                if(std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    logger_.warning("Invalid input! Please enter numbers only.");
                    std::cout << "Move by (x y): ";
                    continue;
                }
                
                int t_x = pos_.x + dx;
                int t_y = pos_.y + dy;
                
                if(t_x < 0 || t_y < 0 || t_x >= grid.getWidth() || t_y >= grid.getHeight() || !grid.IsFreeCell(t_x,t_y)){
                    logger_.warning("Invalid coordinates! Try again.");
                    std::cout << "Move by (x y): ";
                    continue;
                }
                
                break; 
            }
            
            Position new_pos = {pos_.x + dx, pos_.y + dy};
            return move(new_pos, grid);
        }

        case action::ChangeAttackType:{
            attack = (attack == attackType::melee) ? attackType::range : attackType::melee;
            logger_.action("Attack type changed!");
            return true;
        }

        case action::Attack: {
            attackEnemy(enemy);
            return true;
        }
        
        case action::GiveASpell: {
            std::cout << "Choose a spell (index): ";
            sleeve_.showCards();
            int idx;
            while (true) {
                std::cin >> idx;
                if (std::cin.fail() || idx < 0 || idx >= sleeve_.getSize()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    logger_.warning("Please enter a valid spell index!");
                    continue;
                }
                break;
            }

            std::cout << "Choose a target (x y): ";
            int x, y;
            while (true) {
                std::cin >> x >> y;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    logger_.warning("Invalid input! Please enter numbers only.");
                    continue;
                }
                if (x < 0 || y < 0 || x >= grid.getWidth() || y >= grid.getHeight()) {
                    logger_.warning("Target out of bounds! Try again.");
                    continue;
                }
                break;
            }

            bool used = sleeve_.useSpell(idx, world, ctx, Position{x, y});
            return used;
        }
        
        default: return true;
    }
}

void Player::save(std::ostream& os) const {
    Character::save(os); 
    sleeve_.save(os);    
}

void Player::load(std::istream& is) {
    Character::load(is);
    sleeve_.load(is);
}

bool Player::act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx, int param1) {
    switch(a) {
        case action::ChangeAttackType:{
            // param1 - new attack type (0 = melee, 1 = range)
            attack = (param1 == 0) ? attackType::melee : attackType::range;
            logger_.action("Attack type changed!");
            return true;
        }
        default:
            return act(a, grid, enemy, world, ctx);
    }
}

bool Player::act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx, int param1, int param2) {
    switch(a) {
        case action::Move: {
            // param1 = dx, param2 = dy
            int t_x = pos_.x + param1;
            int t_y = pos_.y + param2;

            if(t_x < 0 || t_y < 0 || t_x >= grid.getWidth() || t_y >= grid.getHeight() || !grid.IsFreeCell(t_x,t_y)){
                logger_.warning("Invalid move coordinates!");
                return false;
            }

            Position new_pos = {t_x, t_y};
            return move(new_pos, grid);
        }
        default:
            return act(a, grid, enemy, world, ctx);
    }
}

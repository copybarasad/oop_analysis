#include "player.h"
#include "game_map.h"
#include  "enemy.h"
#include <cmath>
#include "entity_type.h"

Player::Player(int start_x, int start_y, int player_health, int player_melee_damage)
    : Entity(EntityType::PLAYER, start_x, start_y, player_health, player_melee_damage),
      combatMode(CombatMode::MELEE),
      meleeDamage(player_melee_damage),
      rangedDamage(player_melee_damage / 2),
      attackRange(1),
      enemiesKilled(0),
      enemiesForSpellReward(3),
      hand(5), 
      maxHealth(player_health){}

bool Player::move(int dx, int dy, GameMap& map) {    
    Position new_pos(position.getX() + dx, position.getY() + dy);
    if (map.isPositionValid(new_pos)) {
        MapCell& cell = map.getCell(new_pos);
        if(cell.getType() != MapCell::Type::WALL && !cell.isUsed()){
            map.getCell(position).setUsed(false);
            map.getCell(position).setEntity(nullptr);
            position = new_pos;
            cell.setUsed(true);
            cell.setEntity(this);
            
            if (cell.getType() == MapCell::Type::LOW) {
                setSkipTurn(true);
            }
            return true;
        }
    }
    return false;
}



bool Player::canRangedAttack(Position targetPos, int move_dx, int move_dy) const {
    int dx_to_target = targetPos.getX() - position.getX();
    int dy_to_target = targetPos.getY() - position.getY();
    
    if (std::abs(dx_to_target) > attackRange || std::abs(dy_to_target) > attackRange) {
        return false;
    }
    
    if (move_dx != 0) {  
        return (dx_to_target / move_dx > 0) && (dy_to_target == 0);  
    }
    else if (move_dy != 0) {  
        return (dy_to_target / move_dy > 0) && (dx_to_target == 0);  
    }
    
    return false;
}

int Player::getAttackDamage() const {
    return (combatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

void Player::switchCombatMode() {
    if (skipNextTurn) {
        skipNextTurn = false;  
    }
    else{
        if (combatMode == CombatMode::MELEE) {
            combatMode = CombatMode::RANGED;
            attackRange = RANGED_ATTACK_RANGE;
        } else {
            combatMode = CombatMode::MELEE;
            attackRange = MELEE_ATTACK_RANGE;
        }
    }
}

std::string Player::getCombatMode() const {
    return combatMode == CombatMode::MELEE ? "Ближний" : "Дальний";
}

std::string Player::getStatusString() const {
    return "HP: " + std::to_string(health) + " | " + 
           "DMG: " + std::to_string(getAttackDamage()) + " | " +
           "Режим: " + getCombatMode() + " | " +
           "Убийства: " + std::to_string(enemiesKilled) + " | " + 
           "Уровень заклинаний: " + std::to_string(getLevel()) ;
}

PlayerHand& Player::getHand() {
    return hand;
}

void Player::onEnemyKilled() {
    enemiesKilled++;
    hand.onEnemyKilled(enemiesKilled);
}

bool Player::attack(int dx, int dy, GameMap& map) {
    if (skipNextTurn) {
        skipNextTurn = false;
        return false;
    }
    
    Position targetPos(position.getX() + dx, position.getY() + dy);
    
    if (!map.isPositionValid(targetPos)) {
        return false;
    }
    
    MapCell& targetCell = map.getCell(targetPos);
    Entity* entity = targetCell.getEntity();
    if (entity && entity->getType() != EntityType::FOLLOWER){
        if (combatMode == CombatMode::MELEE) {
            entity ->takeDamage(getAttackDamage());
            if (!entity->isAlive()) {
                if(entity->getType() == EntityType::ENEMY){
                    this->onEnemyKilled();
                }
                targetCell.setEntity(nullptr);
                targetCell.setUsed(false);
            }
            return true;
        }
        else {
            for (int y = position.getY() - attackRange; y <= position.getY() + attackRange; y++) {
                for (int x = position.getX() - attackRange; x <= position.getX() + attackRange; x++) {
                    Position checkPos(x, y);
                    if (map.isPositionValid(checkPos) && canRangedAttack(checkPos, dx, dy)) {
                        MapCell& checkCell = map.getCell(checkPos);
                        entity->takeDamage(getAttackDamage());
                        if (!entity->isAlive()) {
                            if(entity->getType() == EntityType::ENEMY){
                                this->onEnemyKilled();
                            }
                            checkCell.setEntity(nullptr);
                            checkCell.setUsed(false);
                        }
                        return true;
                        
                    }
                }
            }
        }
    }
    return false;
}

void Player::update(int new_x, int new_y) {
    position = Position(new_x, new_y);
    health = maxHealth;
}

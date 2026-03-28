#include "entity.h"
#include "game_map.h"

Entity::Entity(EntityType entityType, int start_x, int start_y, int enemy_health, int enemy_damage, bool skipNextTurn) 
    : position(start_x, start_y), health(enemy_health), damage(enemy_damage), 
      skipNextTurn(skipNextTurn), type(entityType) {  
}

Position& Entity::getPosition() {
    return position;
}

const Position& Entity::getPosition() const {
    return position;
}

int Entity::getHealth() const { return health; }
int Entity::getDamage() const { return damage; }
int Entity::getLevel() const { return level; }
void Entity::levelUp() { level++; }

bool Entity::isAlive() const { return health > 0; }

void Entity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

std::string Entity::getStatusString() const{
    return "HP: " + std::to_string(health) + " | " + "DMG: " + std::to_string(damage);
}

void Entity::makeMove(GameMap& map, Position targetPos) {
    if (skipNextTurn) {
        setSkipTurn(false);
        return;
    }
        
    int dx = targetPos.getX() - position.getX();
    int dy = targetPos.getY() - position.getY();

    Position moves[2] = {position, position};
    
    if (abs(dx) > abs(dy)) {
        moves[0] = Position(position.getX() + (dx > 0 ? 1 : -1), position.getY());
        moves[1] = Position(position.getX(), position.getY() + (dy > 0 ? 1 : -1));
    } else {
        moves[0] = Position(position.getX(), position.getY() + (dy > 0 ? 1 : -1));
        moves[1] = Position(position.getX() + (dx > 0 ? 1 : -1), position.getY());
    }

    Position new_pos = position;
    bool canMove = false;

    for (int i = 0; i < 2; i++) {
        if (map.isPositionValid(moves[i])) {
            MapCell& cell = map.getCell(moves[i]);
            if (!cell.isUsed() && cell.getType() != MapCell::Type::WALL) {
                new_pos = moves[i];
                canMove = true;
                break;
            }
        }
    }

    if (canMove) {
        MapCell& old_cell = map.getCell(position);
        MapCell& new_cell = map.getCell(new_pos);
        old_cell.setEntity(nullptr);
        old_cell.setUsed(false);
        
        position = new_pos;
        
        new_cell.setEntity(this);
        new_cell.setUsed(true);
        if( new_cell.getType() == MapCell::Type::TRAP){
            this->takeDamage(new_cell.getTrapDamage());
            new_cell.setType(MapCell::Type::EMPTY);
        }
        else if (new_cell.getType() == MapCell::Type::LOW) {
            setSkipTurn(true);
        }
    }
}

bool Entity::canAttack(Position targetPos){
    if (skipNextTurn) {
        setSkipTurn(false);
        return false;
    }
    int dx = abs(position.getX() - targetPos.getX());
    int dy = abs(position.getY() - targetPos.getY());
    return (dx <= 1 && dy <= 1) && (dx + dy == 1);  
}



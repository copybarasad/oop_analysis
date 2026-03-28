#ifndef ENTITY_H
#define ENTITY_H

#include "entity_type.h"
#include "position.h"
#include <string>

class GameMap;

class Entity {
protected:
    Position position;
    int health;
    int damage;
    bool skipNextTurn;
    EntityType type;
    int level = 1;
    int maxlevel = 1;

public:
    
    Entity(EntityType entityType, int start_x = 0, int start_y = 0, int enemy_health = 50, int enemy_damage = 5, bool skipNextTurn = false);
    Position& getPosition();  
    const Position& getPosition() const; 
    int getHealth() const;
    int getDamage() const;
    bool isAlive() const;
    void takeDamage(int amount);
    bool shouldSkipTurn() const { return skipNextTurn; }
    void setSkipTurn(bool skip) { skipNextTurn = skip; }
    virtual std::string getStatusString() const; 
    void makeMove(GameMap& map, Position targetPos);
    bool canAttack(Position targetPos);
    EntityType getType() const { return type; } 
    virtual void onKill() {}
    virtual ~Entity() = default;
    int getLevel() const;
    void setLevel(int lev){
        level = lev;
    }
    void levelUp();
    int getMaxLevel(){
        return maxlevel;
    }
    void addLevelSpeel(){
        maxlevel += 1;
        level += 1;
    }

};

#endif
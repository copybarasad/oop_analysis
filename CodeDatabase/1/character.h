#pragma once
#include "position.h"
#include "direction.h"
#include "cell_type.h"
#include <string>

class Field;

class Character {
protected:
    std::string name_;
    int health_;
    Position position_;
    bool baseMoveWithField(int dx, int dy, Field& field);

public:
    Character(const std::string& name, int health, const Position& position);
    virtual ~Character() = default;
    
    std::string getName() const;
    int getHealth() const;
    Position getPosition() const;
   
    void setHealth(int health);
    
    virtual void takeDamage(int amount) = 0;
    bool isAlive() const;
    
    bool moveWithField(int dx, int dy, Field& field);
    bool moveWithField(Direction direction, Field& field);
    
    virtual void applyCellEffect(CellType cell_type) = 0;
    
    //lb3
    void moveToPosition(const Position& newPosition) { position_ = newPosition; }
};
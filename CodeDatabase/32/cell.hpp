#ifndef CELL_H
#define CELL_H

enum class CellType{
    kEmpty,
    kPlayer,
    kEnemy,
    kWall,
    kPotionSmall,
    kPotionLarge, 
    kEnemyBuilding,
    kSlowCell,
    kTrap,
    kEnemyTower,
    kAlly
};

class Cell{
private:
    CellType type_;
public:
    Cell() : type_(CellType::kEmpty){}
    explicit Cell(CellType type) : type_(type) {}

    CellType type() const {return type_;}
    void SetType(CellType type) {type_ = type;}

    char Symbol() const;
};

#endif
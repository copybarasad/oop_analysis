#pragma once
#include <memory>

enum class CellType {
    EMPTY,
    WALL,
    SLOW
};

class Player;
class Enemy;
class EnemyBase;
class EnemyTower;
class Ally;

class Cell {
private:
    CellType type_;
    bool hasTrap_;
    std::shared_ptr<Player> player_;
    std::shared_ptr<Enemy> enemy_;
    std::shared_ptr<EnemyBase> base_;
    std::shared_ptr<EnemyTower> tower_;
    std::shared_ptr<Ally> ally_;

public:
    explicit Cell(CellType type = CellType::EMPTY);
    CellType getType() const;
    bool isPassable() const;
    bool isSlow() const;
    bool hasTrap() const;
    void setTrap(bool trap = true);

    std::shared_ptr<Player> getPlayer() const;
    void setPlayer(std::shared_ptr<Player> p);

    std::shared_ptr<Enemy> getEnemy() const;
    void setEnemy(std::shared_ptr<Enemy> e);

    std::shared_ptr<EnemyBase> getBase() const;
    void setBase(std::shared_ptr<EnemyBase> b);

    std::shared_ptr<EnemyTower> getTower() const;
    void setTower(std::shared_ptr<EnemyTower> t);

    std::shared_ptr<Ally> getAlly() const;
    void setAlly(std::shared_ptr<Ally> a);

    bool isEmpty() const;
};
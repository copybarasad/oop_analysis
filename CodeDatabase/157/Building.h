#ifndef BUILDING_H
#define BUILDING_H

class Player;
class GameField;

class Building {
public:
    Building(int x, int y, int hp, bool enemyBuilding);
    virtual ~Building() = default;
    int x() const { return x_; }
    int y() const { return y_; }
    int hp() const { return hp_; }  // Добавлен getter
    void takeDamage(int d);
    bool isDestroyed() const { return hp_ <= 0; }
    bool isEnemyBuilding() const { return enemyBuilding_; }
    virtual bool tryAttack(Player& player, GameField& field) { return false; }
    virtual void tick() {}
private:
    int x_, y_;
    int hp_;
    bool enemyBuilding_;
};
#endif // BUILDING_H

#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <memory>
#include "Unit.h"
#include "Building.h"

class Player;

struct Trap {
    int x, y;
    int damage;
};

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField&) = delete;
    GameField& operator=(const GameField&) = delete;
    int width() const { return width_; }
    int height() const { return height_; }
    void setPlayer(Player* p) { player_ = p; }
    Player* player() { return player_; }
    const Player* player() const { return player_; }

    bool isInside(int x, int y) const;
    void addUnit(std::unique_ptr<Unit> u);
    Unit* unitAt(int x, int y);
    const Unit* unitAt(int x, int y) const;
    void removeDeadUnits();
    void addBuilding(std::unique_ptr<Building> b);
    Building* buildingAt(int x, int y);
    const Building* buildingAt(int x, int y) const;
    bool placeTrap(int x, int y, int damage);
    void checkTrapTrigger(int x, int y);
    void tick();
    void debugPrint() const;
private:
    int width_, height_;
    std::vector<std::unique_ptr<Unit>> units_;
    std::vector<std::unique_ptr<Building>> buildings_;
    std::vector<Trap> traps_;
    Player* player_ = nullptr;
};
#endif // GAMEFIELD_H

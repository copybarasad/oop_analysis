#pragma once
#include <LivingEntity.hpp>
#include <vector>
#include <Ceil.hpp>

class DecoyAlly : public LivingEntity {
public:
    DecoyAlly(int x, int y, int targetX, int targetY);

    void update(std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int height, int width);
    virtual void draw() override; 
    virtual int getExp() const override;

    int getTargetX() const;
    int getTargetY() const;
    int getLifeTimeCounter() const;
    bool getReachedTarget() const;

    void setState(int lifetime, bool reached);

private:
    int targetX;
    int targetY;
    char sprite;
    int lifetimeCounter;
    bool reachedTarget;

    bool isPassable(const std::vector<std::vector<Ceil>>& ceils, int n_x, int n_y, const ScreenSize* screen, int height, int width) const;
    void move(std::vector<std::vector<Ceil>>& ceils, int nextX, int nextY, const ScreenSize* screen, int height, int width);

};
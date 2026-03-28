#pragma once
#include <LivingEntity.hpp>
#include <Constants.hpp>
#include <Player.hpp>
#include <memory>

class Enemy : public LivingEntity {
public:
    Enemy(int x, int y, int level);
    
    void update(std::vector<std::vector<Ceil>>& ceils, 
                const ScreenSize* screen, 
                const std::vector<std::shared_ptr<LivingEntity>>& allEntities,
                int height, int width);
    virtual void draw() override;
    void takeDamage(int amount);
    virtual int getExp() const override;
    int getLvl() const;
    int attack();
    void move(std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int newX, int newY, int height, int width);
    bool isPassable(const std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int nx, int ny, int height, int width) const;

private:
    int damage;
    int level;
    char sprite;
};
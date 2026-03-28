#pragma once
#include "LivingEntity.hpp"
#include <string>
#include <vector>      
#include <memory>      

#include <Player.hpp>   
#include <TowerShot.hpp>   
#include <Ceil.hpp>    
#include <ScreenSize.hpp>

class ArcherTower : public LivingEntity {
public:
    ArcherTower(int x, int y);

    void update(std::shared_ptr<Player> player, const std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int height, int width);

    virtual void draw() override;
    virtual int getExp() const override;

    std::string getStateAsString() const;
    int getCurrentCooldown() const;
    void setState(const std::string& stateStr, int cooldown);

private:
    enum class State {
        IDLE,    
        AIMING,  
        COOLDOWN 
    };

    State currentState;
    int attackCooldown;
    int currentCooldown;
    int attackRadius;
    char sprite;
    TowerShot attackSpell; 

    bool isPlayerInRange(int playerX, int playerY) const;
};
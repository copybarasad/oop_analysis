#pragma once

struct CombatAttributes {
    int hitpoints;
    int maxHitpoints;
    int attackPower;
    
    CombatAttributes(int maxHp, int attack) 
        : hitpoints(maxHp), maxHitpoints(maxHp), attackPower(attack) {}
        
    void takeDamage(int damage) {
        hitpoints = (damage >= hitpoints) ? 0 : hitpoints - damage;
    }
    
    bool isAlive() const {
        return hitpoints > 0;
    }
    
    int getMaxHitpoints() const {
        return maxHitpoints;
    }
};
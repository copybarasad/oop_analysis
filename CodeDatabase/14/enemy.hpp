#ifndef ENEMY_H
#define ENEMY_H
#include "entity.hpp"
#include "defines.hpp" 

class Enemy : public Entity {
private:
    int health;
    int damage;
public:
    Enemy(int x, int y, int start_health = ENEMY_DEFAULT_HEALTH, int enemy_damage = ENEMY_DEFAULT_DAMAGE);
    int getHealth() const override;
    int getDamage() const;
    void takeDamage(int dmg) override;
    void onDeath(Player* player) override;
    bool isAlive() const;
    bool isEnemy() const override;

    EntityType getEntityType() const override { return EntityType::ENEMY; }

    virtual void removeFromBoard(GameBoard& board) override;
    
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
};
#endif
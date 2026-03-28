#pragma once
#include <Entity.hpp>
#include <memory>

class LivingEntity : public Entity, public std::enable_shared_from_this<LivingEntity> {
public:
    LivingEntity(int x, int y, Entity::Type type, int health, int expValue);
    int getHealth() const override;
    virtual void takeDamage(int amount);
    virtual int getExp() const;
    virtual void setHealth(int value);
    virtual void setExp(int value);
    
protected:
    int health;
    int exp;
};
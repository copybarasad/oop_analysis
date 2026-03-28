#ifndef I_ATTACKER_HPP
#define I_ATTACKER_HPP

class GameObject;

class IAttacker {
public:
    virtual ~IAttacker() = default;
    
    virtual int getAttackDamage() const = 0;
    virtual bool canAttack() const = 0;
    virtual void performAttack(GameObject& target) = 0;
    virtual void resetAttackFlag() = 0;
};

#endif
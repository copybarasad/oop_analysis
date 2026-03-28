#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
    protected:
    size_t id;
    unsigned int maxHealth;
    unsigned int currentHealth;
    unsigned int damage;
    bool skipNextTurn;

    Entity (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue, bool skipNextTurn=0);
    
    public:
    virtual ~Entity() = default;

    void setID (size_t id);
    void setMaxHealth (unsigned int maxHealthValue);
    void setCurrentHealth (unsigned int currentHealthValue);
    void setDamage (unsigned int damageValue);
    void setSkip ();
    void restoreSkip ();

    size_t getID () const;
    unsigned int getMaxHealth () const;
    unsigned int getCurrentHealth () const;
    bool isSkipping () const;

    virtual unsigned int calculateDamage () const = 0;
};

#endif
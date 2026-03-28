#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
    int health;
    int maxHealth;
    int damage;
    int x, y;
    char icon = 'C';
    bool isBurn=false;
    int timeLeft = 0;
public:
    Character(int startHealth, int startDamage, int startX, int startY);
    virtual ~Character() = default; 
    
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getDamage() const { return damage; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int nx){x=nx;}
    void setY(int ny){y=ny;}
    
    virtual char getIcon() { return icon; }
    
    void setPosition(int newX, int newY);
    void setHealth(int newHealth) { health = newHealth; }
    void setMaxHealth(int newHealth){maxHealth = newHealth;} 
    void setDamage(int newDamage) { damage = newDamage; }
    
    void takeDamage(int damageAmount);
    void heal(int healAmount);
    bool isAlive() const { return health > 0;}
    bool isBurning(){return isBurn;}
    int getBurnCooldown(){return timeLeft;}
    void setBurnCooldown(int newC){timeLeft=newC;}
    void setAblaze();
    void updateBurning();
    virtual int attack();
    
};

#endif
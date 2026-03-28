#ifndef BEAST_H
#define BEAST_H

class Beast {
    private:
        int hp;
        int damage;
        bool isActive;
    public:
        Beast(int hp = 30, int damage = 10);
        
        void takeDamage(int damageAmount);
        void setActive(bool active);
        bool isAlive() const;

        int getHealth() const;
        int getDamage() const;
        bool getIsActive() const;
};

#endif
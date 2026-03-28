#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

enum class CombatStyle {
    MELEE,
    LR
};
std::ostream& operator<<(std::ostream& os, CombatStyle style);

class Character {
protected:
    int id;
    std::string name;
    int hp;
    int max_hp;
    CombatStyle combatStyle;
    int posX;
    int posY;
    bool isAlive;

    void validateHP() const;
    void validateDamage() const;
    int calculateActualDamage() const;

public:
    Character(int characterId, const std::string& CharacterName, int initialHP, 
        CombatStyle style, int startX, int startY);
    virtual ~Character() = default;

    int getId() const;
    std::string getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getDamage() const;
    CombatStyle getCombatStyle() const;
    int getPosX() const;
    int getPosY() const;
    bool getisAlive() const;

    void setPos(int newX, int newY);
    void setCombatStyle(CombatStyle newStyle);
    void setHP(int newHP);

    void takeDamage(int damageAmount);
    void heal(int healAmount);

    virtual void save(std::ostream& out) const;
    virtual void load(std::istream& in);

    virtual void move(int deltaX, int deltaY) = 0;
    virtual void attack(Character& target) = 0;
    virtual bool canMoveTo(int targetX, int targetY) const = 0;
};
#endif

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "../Entity/Entity.hpp"
#include "../../Items/Item.hpp"
#include "../../Items/Weapon.hpp"
#include "../../Items/Armor.hpp"

class Player : public Entity {
    private:
    unsigned int points;
    std::vector<Item*> inventory;
    Weapon* weapon;
    Armor* armor;
    CombatMode mode;

    public:
    Player (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue, unsigned int pointsValue,
        std::vector<Item*> startInventory, Weapon* startWeapon, Armor* startArmor, CombatMode modeValue);
    
    void setWeapon (Weapon* weapon);
    void setArmor (Armor* armor);
    void setMode (std::string modeValue);
    void setMode (CombatMode modeValue);

    unsigned int getPoints () const;
    const std::vector<Item*>& getInventory () const;
    Weapon* getWeapon () const;
    Armor* getArmor () const;
    CombatMode getMode () const;

    void addPoints (unsigned int bonusPoints);
    void subPoints (unsigned int spentPoints);
    unsigned int calculateDamage () const override;
    unsigned int calcuateArmor () const;
    bool pickUpItem (Item* item);
};

#endif
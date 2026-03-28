#ifndef TOWER
#define TOWER

#include "../../Entity/Entity.hpp"
#include "../../../Field/Field/Field.hpp"

class Tower: public Entity {
    private:
    unsigned int spellCoolDown;
    unsigned int currentCoolDown;
    size_t range;

    public:
    Tower (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue,
        unsigned int spellCoolDown, unsigned int currentCoolDown, size_t range, unsigned int damageValue=0);

    size_t getRange() const;
    unsigned int getCurrentCoolDown() const;
    unsigned int getSpellCoolDown () const;
    void subCoolDown ();
    std::pair<bool, size_t> attack (Field& field, size_t x, size_t y);
    unsigned int calculateDamage () const override;

    private:
    bool checkPosition (Field& field, int targetX, int targetY) const;
};

#endif
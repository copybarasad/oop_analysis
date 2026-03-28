#ifndef GAME_SPELLTOWER_H
#define GAME_SPELLTOWER_H

#include "../basics/Object.h"
#include "../Spells/SpellManager.h"
#include "../Spells/DirectDamageSpellEnemy.h"

class SpellTower : public Object {
private:
    bool is_ready_ = false;
    bool is_active_ = false;
    int sells_amount_;
    SpellManager spell_book_;

public:
    explicit SpellTower(const int spells_amount = 3) : Object("SpellTower", ObjTypes::Neither),
                                                       sells_amount_(spells_amount) {
    };

    void update() override;

    void activateSpellBook();

    bool isReady() const;

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif

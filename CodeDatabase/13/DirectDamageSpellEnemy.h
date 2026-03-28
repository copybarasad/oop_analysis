#ifndef GAME_DIRECTDAMAGESPELLENEMY_H
#define GAME_DIRECTDAMAGESPELLENEMY_H
#include "DirectDamageSpell.h"

class DirectDamageSpellEnemy : public DirectDamageSpell {
public:
    DirectDamageSpellEnemy():DirectDamageSpell(){
        this->damage_ /= 2;
        this->targetType_ = ObjTypes::Friendly;
        this->cost_ = 4;
    };
};
#endif
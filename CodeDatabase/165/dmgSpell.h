#ifndef DMGSPELLH
#define DMGSPELLH
#include "spell.h"
class dmgSpell: public spell{
    size_t dmg;
public:
    void setDmg(size_t);
    size_t getDmg();
};
#endif
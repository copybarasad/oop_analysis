#include "Spell.h"
#include "Entity.h"
#include "Enemy.h"

class TrapSpell: public Spell{

public:
    void cast(std::string direction, Gamefield* gf,int pl_X, int pl_Y) override;


    TrapSpell();


};
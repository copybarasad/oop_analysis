#ifndef ICESPEARH
#define ICESPEARH
#include "dmgSpell.h"
class iceSpear : public dmgSpell{
public:
    std::vector<coords> targets(map*, hero*);
    void use(map*, hero*);
    void setRank(size_t);
    iceSpear(size_t);
    iceSpear();
};
#endif
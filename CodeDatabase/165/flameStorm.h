#ifndef FLAMESTORMH
#define FLAMESTORMH
#include "dmgSpell.h"
class flameStorm : public dmgSpell{
    size_t blastRange;
public:
    std::vector<coords> targets(map*, hero*);
    void use(map*, hero*);
    void setBlastRange(size_t);
    void setRank(size_t);
    size_t getBlastRange();
    flameStorm(size_t);
    flameStorm();
};
#endif
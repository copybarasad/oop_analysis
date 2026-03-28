#ifndef IUSEH
#define IUSEH
#include "../abstractNinterfaces/coords.h"
#include <vector>
class map;
class hero;

class iUse{
public:
    virtual std::vector<coords> targets(map*, hero*) = 0;
    virtual void use(map*, hero*) = 0;
    
};
#endif
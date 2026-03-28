#ifndef SPELLH
#define SPELLH
#include <string>
#include "iUse.h"
class spell : public iUse{
    size_t range;
    std::string name;
protected:
    size_t rank;
public:
    void setName(std::string);
    void setRange(size_t);
    virtual void setRank(size_t) = 0;
    std::string getName();
    size_t getRange();
    size_t getRank();
};
#endif
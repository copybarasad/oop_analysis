#ifndef IAREASPELL_H
#define IAREASPELL_H

#include "ISpell.h"

class IAreaSpell : public virtual ISpell { 
public:
    virtual ~IAreaSpell() = default;
    virtual int getAreaOfEffect() const = 0;
    virtual void setAreaOfEffect(int area) = 0;
};

#endif
#pragma once
#include "IDamager.h"

class ISwitchingDamager : public IDamager
{
public:
    virtual void SwitchDamageMode(int index) = 0;

    virtual int GetCurrentModeIndex() const = 0;
};
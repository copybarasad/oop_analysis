#pragma once

class IDamager
{
public:
    virtual ~IDamager() = default;

    virtual void ChangeDamageByValue(int value) = 0;

    virtual int GetDamage() = 0;
};
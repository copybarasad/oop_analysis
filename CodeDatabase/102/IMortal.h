#pragma once

class IMortal
{
public:
    virtual ~IMortal() = default;

    virtual void TakeDamage(int damage) = 0;

    virtual int GetHealth() = 0;

    virtual int GetMaxHealth() = 0;

    virtual bool IsAlive() = 0;
};
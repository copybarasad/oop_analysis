//
// Created by Artem on 03.10.2025.
//

#ifndef LABA1_LONGRANGEATTACK_H
#define LABA1_LONGRANGEATTACK_H

#include <string>
#include <map>
#include "Attack.h"

class LongRangeAttack : public Attack{
public:
    LongRangeAttack(int damage, int range) : Attack(damage, AttackType::LongRange), range(range){}

    virtual int getRange() const noexcept final;

    virtual std::string serialize() noexcept final;
    static LongRangeAttack deserialize(std::map<std::string, std::string> json) noexcept;
protected:
    int range;
};


#endif //LABA1_LONGRANGEATTACK_H

//
// Created by Artem on 03.10.2025.
//

#ifndef LABA1_CLOSERANGEATTACK_H
#define LABA1_CLOSERANGEATTACK_H

#include <string>
#include <map>
#include "Attack.h"

class CloseRangeAttack : public Attack{
public:
    CloseRangeAttack(int damage) : Attack(damage, AttackType::CloseRange){}

    virtual std::string serialize() noexcept final;
    static CloseRangeAttack deserialize(std::map<std::string, std::string> json) noexcept;
};


#endif //LABA1_CLOSERANGEATTACK_H

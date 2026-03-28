#pragma once
#include <string>

enum class AttackMode { Melee, Ranged };

std::string attackModetoString(AttackMode mode);
AttackMode stringToAttackMode(const std::string& s);
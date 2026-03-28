#include "potion.hpp"

Potion::Potion(const std::string& name, int heal_amount)
    : name_(name), heal_amount_(heal_amount) {}

std::string Potion::GetName() const {return this->name_;}
int Potion::GetHealAmount() const{return this->heal_amount_;}
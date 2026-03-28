#include "Armor.hpp"

Armor::Armor (std::string name, unsigned int armorClassValue) :
    Item(name), armorClass(armorClassValue) {}

unsigned int Armor::getArmorClass () const { return armorClass; }
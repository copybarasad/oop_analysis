#ifndef ARMOR_H
#define ARMOR_H

#include "Item.hpp"

class Armor : public Item {
    private:
    unsigned int armorClass;

    public:
    Armor (std::string name, unsigned int armorClassValue);

    unsigned int getArmorClass () const;
};

#endif
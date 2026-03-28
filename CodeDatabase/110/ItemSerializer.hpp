#ifndef ITEM_SER_H
#define ITEM_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Items/Item.hpp"
#include "../../../../../Items/Weapon.hpp"
#include "../../../../../Items/Armor.hpp"

class ItemSerializer : public Serializable {
    private:
    const Item& item;
    
    public:
    ItemSerializer (const Item& item);
    
    std::string serialize () const override;
};

#endif
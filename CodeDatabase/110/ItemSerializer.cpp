#include "ItemSerializer.hpp"

ItemSerializer::ItemSerializer (const Item& item) :
    item(item) {}

std::string ItemSerializer::serialize () const {
    std::ostringstream oss;
    oss << "ITEM:" << item.getName();

    if (dynamic_cast<const Weapon*>(&item)) {
        const Weapon& weapon = *dynamic_cast<const Weapon*>(&item);
        oss << ",WEAPON,";
        oss << "type:" << (weapon.getType() == CombatMode::CLOSE ? "CLOSE" : "RANGED") << ",";
        oss << "minDamage:" << weapon.getMinDamage() << ",";
        oss << "maxDamage:" << weapon.getMaxDamage() << ",";
        oss << "critChance:" << weapon.getCritChance() << ",";
        oss << "critBonus:" << weapon.getCritBonus() << ",";
        oss << "range:" << weapon.getRange();
    }
    else if (dynamic_cast<const Armor*>(&item)) {
        const Armor& armor = *dynamic_cast<const Armor*>(&item);
        oss << ",ARMOR,armorClass:" << armor.getArmorClass();
    }
    else {
        oss << ",BASIC";
    }
    
    return oss.str();
}
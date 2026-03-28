#include "PlayerSerializer.hpp"

PlayerSerializer::PlayerSerializer (const Player& player) :
    player(player) {}

std::string PlayerSerializer::serialize () const {
    std::ostringstream oss;
    oss << "type:PLAYER,";
    oss << "points:" << player.getPoints() << ",";
    oss << "mode:" << (player.getMode() == CombatMode::CLOSE ? "CLOSE" : "RANGED") << ",";
    
    if (player.getWeapon()) {
        ItemSerializer weaponSerializer(*player.getWeapon());
        oss << "weapon:" << weaponSerializer.serialize() << ",";
    }
    else {
        oss << "weapon:NONE,";
    }
    
    if (player.getArmor()) {
        ItemSerializer armorSerializer(*player.getArmor());
        oss << "armor:" << armorSerializer.serialize() << ",";
    } else {
        oss << "armor:NONE,";
    }
    
    oss << "inventory:";
    const auto& inventory = player.getInventory();
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (i > 0) oss << ";";
        ItemSerializer itemSerializer(*inventory[i]);
        oss << itemSerializer.serialize();
    }
    
    return oss.str();
}
#ifndef ITEM_DESER_H
#define ITEM_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"

class ItemDeserializer : public BaseDeserializer {
    public:
    Item* deserialize(const std::string& data);

    private:
    Weapon* deserializeWeapon(const std::string& name, const std::vector<std::string>& parts);
    Armor* deserializeArmor(const std::string& name, const std::vector<std::string>& parts);
    std::string combineParts(const std::vector<std::string>& parts, size_t startIndex) const;
};

#endif
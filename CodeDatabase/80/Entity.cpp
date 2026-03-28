#include "Entity.h"
#include "Map.h"  
#include <sstream>
#include <string>

void Entity::SetCoord(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}

void Entity::GetDirection(char dir, int& dx, int& dy) const {
    auto it = directions.find(dir);
    if (it != directions.end()) {
        dx = it->second.first;
        dy = it->second.second;
    }
    else {
        dx = 0; dy = 0;
    }
}

void Entity::TakeDamage(int damage, Map& map) {
    heallpoints -= damage;
    if (heallpoints <= 0) {
        map.SetVal(x, y, '0', nullptr);
    }
}
std::string Entity::SaveToString() const {
    std::ostringstream ss;
    ss << "x:" << x << "\n";
    ss << "y:" << y << "\n";
    ss << "type:" << ind_type << "\n";
    ss << "hp:" << heallpoints << "\n";
    return ss.str();
}

void Entity::LoadFromString(const std::string& data) {
    std::istringstream ss(data);
    std::string line;

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        if (key == "x") x = std::stoi(value);
        else if (key == "y") y = std::stoi(value);
        else if (key == "type") ind_type = value[0];
        else if (key == "hp") heallpoints = std::stoi(value);
    }
}

std::string Entity::GetSaveHeader() const {
    return "ENTITY";
}
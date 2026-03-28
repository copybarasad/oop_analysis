#pragma once

#include "Being.h"
#include <string>
#include "json.hpp"
using json = nlohmann::json;

class Player : public Being {
private:
    int points = 0;
    bool bow_equipped = false;
    int bow_damage;

public:
    Player(int damage, Coordinates position, int maxHealth = 50);
    Player(int damage, int bow_damage, Coordinates position, int maxHealth = 50);
    ~Player() override = default;

    std::string getHud() const;
    float attack() override;
    void toggleBow();
    void setBow(bool on);
    bool isBowEquipped() const;

    void changePoints(int diff);
    int getPoints() const;
    void setBowDamage(int d) { bow_damage = d; }
    int getBowDamage() const { return bow_damage;
}
    json toJson() const;
    void fromJson(const json& j);

};
#ifndef PLACE_H
#define PLACE_H

#include <string>
#include "game_objects.h"

class Place {
private:
    int type;
    std::string effect;
    std::string Name;

    
public:
    Place();
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);

    int getType();
    std::string getEffect();
    std::string getName();
    void setEffect(std::string new_effect);
    void setType(int new_type);

};

#endif
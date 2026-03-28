#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <string>
#include <nlohmann/json.hpp>

class GameObject {
protected: 
    int x;
    int y;
    int HP;
    std::string type_combat;
    int ranged;
    bool flag_life;
    int frozen;
    
public:
    GameObject(int X, int Y, int hp, std::string combat, int RANGED, int froz);
    virtual ~GameObject() = default;
    virtual int getDamage() = 0;
    void move(int nx, int ny);
    int getX();
    int getY();

    virtual nlohmann::json toJson() const;
    virtual void fromJson(const nlohmann::json& j);

    std::string getTypeCombat();
    void setTypeCombat(std::string types);
    int getFrozen();
    void setFrozen(int n);
    void reduceFrozen();
    int getRange();
    void takeDamage(int received_damage);
    int getHP();
};

#endif
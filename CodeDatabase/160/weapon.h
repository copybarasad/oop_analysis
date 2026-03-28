#ifndef INCLUDE_WEAPON
#define INCLUDE_WEAPON

#include <string>

enum class WeaponType {
    Sword,
    Bow
};

class Weapon {
private:
    WeaponType type_;
    int damage_;
    int range_;
    std::string name_;

public:
    Weapon();
    Weapon(WeaponType type);

    WeaponType type() const;
    int damage() const;
    int range() const;
    std::string name() const;

    void set_type(WeaponType type);
};

#endif
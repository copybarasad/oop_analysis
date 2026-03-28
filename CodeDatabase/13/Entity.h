#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <vector>
#include <memory>
#include "Object.h"
#include "Weapon.h"
#include "ObjTypes.h"


class Entity : public Object {
private:
    int points_;
    int hp_;
    std::vector<Weapon *> weapons_;
    size_t stuned_for_ = 0;

public:
    Entity(const std::string &name, const int hp, const ObjTypes type,
           const int points, std::vector<Weapon *> weapons)
        : Object(name, type), points_(points), hp_(hp),
          weapons_(std::move(weapons)) {
    };

    explicit Entity(const std::string &name = "Entity", const int hp = 1,
                    const ObjTypes type = ObjTypes::Neither, const int points = 0)
        : Object(name, type),
          points_(points), hp_(hp), weapons_{new Weapon(this)} {
    };

    ~Entity() override;

    void addStun(const int turns = 1);

    int getStun() const;

    virtual void onDeath();

    virtual int attack(Object *target);

    void changeWeapon();

    void addWeapon(Weapon *weapon);

    void deleteWeapon();

    Weapon *getWeapon() const;

    virtual void onHit(int damage);

    int move(int dx, int dy);

    int getHp() const;

    void saveToJson(nlohmann::json *savefile) override;
    void loadFromJson(nlohmann::json *savefile) override;

};


#endif

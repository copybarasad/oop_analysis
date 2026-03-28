#ifndef UNTITLED_ENTITY_H
#define UNTITLED_ENTITY_H



#include <memory>
#include <string>

struct Coord;
class Field;

class Entity {
public:
    Entity(int hp, int damage, std::string name);
    virtual ~Entity();

    virtual std::shared_ptr<Entity> clone() const = 0;

    virtual void onTurn(Field& field, const Coord& pos);

    virtual void takeDamage(int amount);

    bool isAlive() const noexcept;
    int hp() const noexcept;
    int damage() const noexcept;
    const std::string& name() const noexcept;

protected:
    int hp_;
    int damage_;
    std::string name_;
};




#endif //UNTITLED_ENTITY_H

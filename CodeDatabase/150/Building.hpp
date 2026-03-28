#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
#include <stdexcept>

class Building {
protected:
    int positionX_;
    int positionY_;
    int health_;

    void validateParameters() const;

public:
    Building(int x, int y, int health);
    virtual ~Building() = default;

    virtual void update() = 0;
    virtual bool isActive() const = 0;

    int getPositionX() const;
    int getPositionY() const;
    int getHealth() const;
    bool isDestroyed() const;
    void takeDamage(int damage);

    virtual void serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&positionX_), sizeof(positionX_));
        os.write(reinterpret_cast<const char*>(&positionY_), sizeof(positionY_));
        os.write(reinterpret_cast<const char*>(&health_), sizeof(health_));
    }
    
    virtual void deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&positionX_), sizeof(positionX_));
        is.read(reinterpret_cast<char*>(&positionY_), sizeof(positionY_));
        is.read(reinterpret_cast<char*>(&health_), sizeof(health_));
    }
};

#endif
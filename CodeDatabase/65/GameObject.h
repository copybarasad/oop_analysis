#pragma once

#include <cstdint>
#include <string>
#include <memory>

enum ObjectTag : uint32_t {
    TAG_NONE = 0,
    TAG_STATIC = 1 << 0,
    TAG_ENEMY = 1 << 1,
    TAG_PLAYER = 1 << 2,
    TAG_TOWER = 1 << 3,
    TAG_SLOW_CELL = 1 << 4,
    TAG_TRAP = 1 << 5,
    TAG_DAMAGE_TOWER = 1 << 6,
    TAG_WALL = 1 << 7,
};

class GameObject {
protected:
    int x, y;
    char sprite;
    std::string color;
    bool solid;
    uint32_t tags = ObjectTag::TAG_NONE;

public:
    GameObject(int x, int y, char sprite, std::string color = "\033[0m",
            bool solid = false, ObjectTag tags = ObjectTag::TAG_NONE)
        : x(x), y(y), sprite(sprite), color(color), solid(solid), tags(tags) {}

    virtual ~GameObject() = default;

    virtual std::unique_ptr<GameObject> clone() const = 0;

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    char getSprite() const { return sprite; }
    std::string getColor() const { return color; }
    bool isSolid() const { return solid; }

    // Setters
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }

    // Tags
    ObjectTag getTags() const { return static_cast<ObjectTag>(tags); }
    
    bool hasTag(ObjectTag tag) {
        return (tags & tag) != 0;
    }

    void addTag(ObjectTag tag) {
        tags |= tag;
    }

    void removeTag(ObjectTag tag) {
        tags &= ~tag;
    }

    bool isEnemy() {
        return hasTag(ObjectTag::TAG_ENEMY) 
            || hasTag(ObjectTag::TAG_TOWER);
    }
};

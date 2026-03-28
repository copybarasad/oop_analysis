#ifndef GAMELB1_ENTITY_H
#define GAMELB1_ENTITY_H

class Entity {
public:
    Entity(int x, int y);
    virtual ~Entity() = default;

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

protected:
    int x_;
    int y_;
};

#endif //GAMELB1_ENTITY_H
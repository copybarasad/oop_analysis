#pragma once
#include <SFML/Graphics.hpp>

class Portal {
public:
    Portal(int x, int y);
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void update(float dt);
    float getRotation() const { return m_rotation; }

private:
    int m_x, m_y;
    float m_rotation;
};
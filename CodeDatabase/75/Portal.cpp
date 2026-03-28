#include "Portal.h"

Portal::Portal(int x, int y) : m_x(x), m_y(y), m_rotation(0.0f) {}

void Portal::update(float dt) {
    m_rotation += dt * 90.0f; 
    if (m_rotation >= 360.0f) m_rotation -= 360.0f;
}
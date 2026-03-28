#include "GameObject.h"

GameObject::GameObject(int health, int maxHealth, int x, int y)
    : m_health(health), m_maxHealth(maxHealth), m_positionX(x), m_positionY(y) {}
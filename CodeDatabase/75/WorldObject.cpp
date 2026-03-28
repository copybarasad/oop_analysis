#include "WorldObject.h"
#include "GameField.h"

WorldObject::WorldObject(int x, int y, Type type, sf::Color color) 
    : m_x(x), m_y(y), m_type(type), m_color(color) {}

bool WorldObject::isSolid() const {
    switch(m_type) {
        case TREE: return true;
        case ROCK: return true;
        case WALL: return true;
        case PILLAR: return true;
        case CRATE: return true;
        case CRYSTAL: return true;
        case WATER: return false; 
        default: return false; 
    }
}

std::string WorldObject::getTextureName() const {
    switch(m_type) {
        case TREE: return "tree";
        case ROCK: return "rock";
        case WATER: return "water";
        case GRASS: return "grass";
        case FLOWER: return "flower";
        case PILLAR: return "wall_tex"; 
        case WALL: return "wall_brick";
        case RUG: return "floor_tex";   
        case RUNE: return "floor_tex";
        case MUSHROOM: return "mushroom";
        case STUMP: return "stump";
        case CRYSTAL: return "crystal";
        case BONES: return "bones";
        case CRATE: return "crate";
        default: return "obstacle.png";
    }
}

float WorldObject::getScale() const {
    if (m_type == TREE) return 1.5f;
    return 1.0f;
}

int WorldObject::getRenderLayer() const {
    if (m_type == GRASS || m_type == FLOWER || m_type == WATER || m_type == RUG || m_type == RUNE || m_type == MUSHROOM || m_type == BONES || m_type == STUMP) return 0;
    if (m_type == TREE) return 2; 
    return 1;
}

void WorldObject::drawOnField(GameField& field) const {
    if (field.isInBounds(m_x, m_y)) {
        if (isSolid()) {
            field.getCell(m_x, m_y).setType(Cell::OBSTACLE);
        }
    }
}

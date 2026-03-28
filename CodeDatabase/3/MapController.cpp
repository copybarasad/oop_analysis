#include "GameClass/MapController.hpp"
#include <iostream>



MapController::MapController(int level) : map(LM.loadLevelMap(level)) {
    MapDTO m = LM.loadLevelMap(level);
    height = m.height;
    width = m.width;
};


MapDTO MapController::toDTO() {
    MapDTO dto;
    dto.height = this->height;
    dto.width = this->width;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            FieldPair field;
            field.coord = {x, y};
            field.type = map.getField(x, y).getStatus();
            dto.FPs.push_back(field);
        }
    }

    return dto;
}

void MapController::loadLevel(int level) {
    MapDTO m = LM.loadLevelMap(level);
    map = Map(m);
    height = m.height;
    width = m.width;
}


int MapController::getWidth() {
    return width;
}

int MapController::getHeight() {
    return height;
}

bool MapController::canStep(Coordinates position, Coordinates delta, FieldType whoseStep) {
    int nx = position.x + delta.x;
    int ny = position.y + delta.y;

    if (nx < 0 || ny < 0) return false;
    if (nx >= width || ny >= height) return false;

    FieldType status = map.getField(nx, ny).getStatus();
    
    if (whoseStep == FieldType::ENEMY) {
        return status != FieldType::FIELD_BLOCK && status != FieldType::SPAWN_TOWER && status != FieldType::ENEMY && status != FieldType::ATTACK_TOWER;
    }

    return status != FieldType::FIELD_BLOCK && status != FieldType::SPAWN_TOWER;
}


std::vector<Coordinates> MapController::getAllowedSteps(Coordinates position, FieldType whoseStep) {
    std::array<Coordinates, 4> deltas = {
        Coordinates{0, -1},
        Coordinates{0,  1},
        Coordinates{-1, 0},
        Coordinates{1,  0}
    };

    std::vector<Coordinates> result;

    for (auto& d : deltas) {
        if (canStep(position, d, whoseStep)) {
            result.push_back(d);
        }
    }

    return result;
}



Field* MapController::getField(int x, int y) {
    if (x >= this->getWidth() || y >= this->getHeight() || x < 0 || y < 0) return nullptr;
    return &this->map.getField(x, y);
}

Field* MapController::getField(Coordinates coord) {
    if (coord.x >= this->getWidth() || coord.y >= this->getHeight() || coord.x < 0 || coord.y < 0) return nullptr;
    return &this->map.getField(coord.x, coord.y);
}
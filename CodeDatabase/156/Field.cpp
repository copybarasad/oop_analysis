//
// Created by Artem on 24.09.2025.
//

#include "Field.h"
#include "../Exceptions/CoordinateException.h"
#include "../Exceptions/Notifications/CellImpassableNotification.h"
#include "../GameSetup/Utils/JsonParser.h"

int Field::getWidth() const noexcept {
    return width;
}

int Field::getHeight() const noexcept {
    return height;
}

std::vector<std::vector<Cell>> &Field::getFieldCells() noexcept {
    return cells;
}


bool Field::canMoveToOrSpawnOn(int x, int y) const {
    this->isCoordsAvailable(x, y);
    this->isCellPassable(x, y);

    const Cell &fieldCell = this->cells[x][y];
    if (fieldCell.hasEntityInCell()) {
        throw CellImpassableNotification("cell is occupied by other entity");
    }

    return true;
}

std::vector<Constants::XYPair> Field::hasNearEntityOfSomeTypes(std::map<EntityType, bool> types, int x,
                                                               int y) const noexcept { //возварщает координаты стоящих в радиусе атаки Entity определённых типов


    std::vector<Constants::XYPair> result;
    for (Constants::dxdy pair: Constants::dxdys) {
        int nX = x + pair.x;
        int nY = y + pair.y;
        if (nX >= 0 && nX < this->height
            && nY >= 0 && nY < this->width) {
            if (this->cells[x + pair.x][y + pair.y].hasEntityInCell() &&
                types.count(this->cells[x + pair.x][y + pair.y].getEntityInCellType()) > 0) {
                result.push_back({x + pair.x, y + pair.y});
            }
        }
    }
    return result;
}

const std::vector<std::vector<Cell>> &Field::getFieldCells() const noexcept {
    return this->cells;
}

bool Field::canMoveToOrSpawnOnNoExcept(int x, int y) const noexcept {
    try {
        return this->canMoveToOrSpawnOn(x, y);
    } catch (...) {
        return false;
    }
}

std::vector<Constants::XYPair>
Field::hasNearEntityOfSomeTypesWithAChance(std::map<EntityType, bool> types, int x, int y, int chance) const noexcept {

    std::vector<Constants::XYPair> result = this->hasNearEntityOfSomeTypes(types, x, y);
    for (auto it = result.begin(); it != result.end();) {
        if ((rand() % 101) > chance) {
            it = result.erase(it);
        } else {
            ++it;
        }
    }

    return result;
}

bool Field::isCoordsAvailable(int x, int y) const {
    if (x < 0 || x >= this->height) {
        throw CoordinateException{"x coordinate must be between 0 and "
                                  + std::to_string(this->height)
                                  + " but x is "
                                  + std::to_string(x)
                                  };
    }

    if (y < 0 || y >= this->width) {
        throw CoordinateException{"y coordinate must be between 0 and "
                                  + std::to_string(this->width)
                                  + " but y is "
                                  + std::to_string(y)
                                  };
    }
    return true;
}

bool Field::isCellPassable(int x, int y) const {
    const Cell &fieldCell = this->cells[x][y];
    if (fieldCell.getCellType() == CellType::Impassable) {
        throw CellImpassableNotification("cell is impassible, wall or something sht like that");
    }
    return true;
}

std::string Field::serialize() {
    std::string res = "{";
    res += "width:";
    res += std::to_string(this->width) + ",";
    res += "height:";
    res += std::to_string(this->height) + ",";
    res += "cells:[";
    for (int i = 0; i < cells.size(); ++i) {
        for (int j = 0; j < cells[0].size(); ++j) {
            res += cells[i][j].serialize();
            if (i != cells.size()-1 || j != cells[0].size()-1){
                res += ",";
            }
        }
    }
    res += "]}";
    return res;
}

Field* Field::deserialize(std::map<std::string, std::string>& json) {
    int height = std::stoi(json.at("height"));
    int width = std::stoi(json.at("width"));
    std::vector<std::vector<Cell>> cells;
    
    std::vector<std::string> cellsStrs = JsonParser::parseJsonArray(json.at("cells"));
    std::vector<Cell> tmp;
    for (int i = 0; i < cellsStrs.size(); ++i) {
        std::map<std::string, std::string> cellMap = JsonParser::parseJsonWithNestedObj(cellsStrs.at(i));
        tmp.push_back(Cell::deserialize(cellMap));
        if ((i+1) % width == 0 && i != 0){
            cells.push_back(tmp);
            tmp = std::vector<Cell>();
        }
    }
    return new Field(width, height, cells);
}
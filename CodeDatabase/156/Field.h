//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_FIELD_H
#define LABA1_FIELD_H
#include <vector>
#include <map>
#include "Cell.h"
#include "../Constants/Constants.h"

class Field {
public:
    Field(const int width, const int height, std::vector<std::vector<Cell>> cells) : width(width),
                                                                                            height(height),
                                                                                            cells(std::move(cells)) {}

    Field(const Field& other): width{other.width}, height(other.height), cells(other.cells){}
    Field(Field&& other): width{other.width}, height(other.height), cells(std::move(other.cells)){}

    Field& operator=(Field&& other) noexcept {
        if (this != &other) {
            *this = Field(std::move(other));
        }
        return *this;
    }

    Field& operator=(const Field& other) {
        if (this != &other) {
            *this = Field(other);
        }
        return *this;
    }

    std::vector<std::vector<Cell>>& getFieldCells() noexcept; //returns instance with modify ability to can modify Cells in vectors


    int getWidth() const noexcept; //return data
    int getHeight() const noexcept; //return data
    const std::vector<std::vector<Cell>>& getFieldCells() const noexcept; //returns instance to read or use static Cell methods
    bool isCoordsAvailable(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    bool canMoveToOrSpawnOn(int x, int y) const;
    bool canMoveToOrSpawnOnNoExcept(int x, int y) const noexcept;
    std::vector<Constants::XYPair> hasNearEntityOfSomeTypes(std::map<EntityType, bool> types, int x, int y) const noexcept;
    std::vector<Constants::XYPair> hasNearEntityOfSomeTypesWithAChance(std::map<EntityType, bool> types, int x, int y, int chance) const noexcept;

    std::string serialize();
    static Field* deserialize(std::map<std::string, std::string>& json);

protected:
    const int width, height;
    std::vector<std::vector<Cell>> cells;

private:
};


#endif //LABA1_FIELD_H

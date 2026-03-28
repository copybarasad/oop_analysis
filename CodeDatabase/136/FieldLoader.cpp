#include "FieldLoader.h"
#include "GameExceptions.h"
#include <iostream>

FieldLoader::FieldLoader(std::ifstream& file, GameField& field)
    : file(file), field(field) {}

bool FieldLoader::loadFieldData() {
    try {
        std::string marker;
        file >> marker;
        
        if (marker != "FIELD_DATA") {
            throw LoadException("field data", "marker not found: " + marker);
        }


        int savedWidth, savedHeight;
        file >> savedWidth >> savedHeight;
        field.resize(savedWidth, savedHeight);

        std::cout << "Loading field data: " << savedWidth << "x" << savedHeight << "\n";
        
        for (int y = 0; y < savedHeight; ++y) {
            for (int x = 0; x < savedWidth; ++x) {
                if (!loadCellData(x, y)) {
                    throw LoadException("field data", 
                        "cell data at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
            }
        }

        file >> marker;
        if (marker != "END_FIELD_DATA") {
            throw LoadException("field data", "end marker not found: " + marker);
        }
        
        std::cout << "Field data loaded successfully\n";
        return true;
        
    } catch (const LoadException& e) {
        throw;
    } catch (const std::exception& e) {
        throw LoadException("field data", std::string("unexpected: ") + e.what());
    }
}

bool FieldLoader::loadCellData(int x, int y) {
    try {
        int cellType, hasTrap, trapDamage;
        file >> cellType >> hasTrap >> trapDamage;
        
        if (file.fail()) {
            throw LoadException("cell data", 
                "read failed at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
        
        if (!validateCellData(cellType, hasTrap, trapDamage)) {
            return false;
        }
        
        Cell& cell = field.getCell(x, y);
        cell.setType(static_cast<CellType>(cellType));
        
        if (hasTrap == 1) {
            cell.setHasTrap(true);
            cell.setTrapDamage(trapDamage);
        } else {
            cell.setHasTrap(false);
            cell.setTrapDamage(0);
        }
        
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("cell data", 
            "validation failed at (" + std::to_string(x) + ", " + std::to_string(y) + "): " + e.what());
    } catch (const std::exception& e) {
        throw LoadException("cell data", 
            "unexpected error at (" + std::to_string(x) + ", " + std::to_string(y) + "): " + e.what());
    }
}

bool FieldLoader::validateCellData(int cellType, int hasTrap, int trapDamage) const {
    if (cellType < 0 || cellType > 2) {
        throw ValidationException("cell type", std::to_string(cellType));
    }
    if (hasTrap != 0 && hasTrap != 1) {
        throw ValidationException("trap flag", std::to_string(hasTrap));
    }
    if (trapDamage < 0) {
        throw ValidationException("trap damage", std::to_string(trapDamage));
    }
    return true;
}

bool FieldLoader::validateFieldMarker(const std::string& expected) {
    try {
        std::string marker;
        file >> marker;
        if (marker != expected) {
            throw ValidationException("field marker", "expected: " + expected + ", got: " + marker);
        }
        return true;
    } catch (const std::exception& e) {
        throw LoadException("field marker", e.what());
    }
}
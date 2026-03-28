#ifndef FIELDLOADER_H
#define FIELDLOADER_H

#include "GameField.h"
#include <fstream>
#include <string>

class FieldLoader {
private:
    std::ifstream& file;
    GameField& field;

public:
    FieldLoader(std::ifstream& file, GameField& field);
    bool loadFieldData();
    
private:
    bool validateFieldMarker(const std::string& expected);
    bool loadCellData(int x, int y);
    bool validateCellData(int cellType, int hasTrap, int trapDamage) const;
};

#endif
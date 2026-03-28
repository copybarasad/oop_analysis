#include "FieldDeserializer.hpp"

Field* FieldDeserializer::deserialize(const std::string& data) {
    std::istringstream stream(data);
    std::string line;
    
    unsigned int width = 0, height = 0;
    bool foundCells = false;
    
    while (std::getline(stream, line)) {
        auto parts = split(line, ':');
        if (parts.size() == 2) {
            if (parts[0] == "width") {
                width = parseUnsignedInt(parts[1], "field width");
            } else if (parts[0] == "height") {
                height = parseUnsignedInt(parts[1], "field height");
            }
        } else if (line == "cells:") {
            foundCells = true;
            break;
        }
    }
    
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        throw ValidationException("Invalid field dimensions");
    }
    if (!foundCells) {
        throw ValidationException("Cells section not found");
    }

    Field* field = new Field(width, height, 0, 0);
    parseAndSetCells(stream, field, width, height);
    
    return field;
}

void FieldDeserializer::parseAndSetCells(std::istringstream& stream, Field* field, unsigned int width, unsigned int height) {
    std::string line;
    CellDeserializer cellDeserializer;

    for (unsigned int y = 0; y < height; ++y) {
        if (!std::getline(stream, line)) {
            throw ValidationException("Unexpected end of cells data");
        }
        
        auto cellDataList = split(line, '|');
        if (cellDataList.size() != width) {
            throw ValidationException("Cell row width mismatch at row " + std::to_string(y));
        }

        for (unsigned int x = 0; x < width; ++x) {
            Cell cell = cellDeserializer.deserialize(cellDataList[x], x, y);
            field->getCell(x, y) = std::move(cell);
        }
    }
}
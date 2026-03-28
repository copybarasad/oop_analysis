#include "FieldSerializer.hpp"

FieldSerializer::FieldSerializer (const Field& field) :
    field(field) {}

std::string FieldSerializer::serialize () const {
    std::ostringstream oss;
    oss << "width:" << field.getWidth() << "\n";
    oss << "height:" << field.getHeight() << "\n";
    
    oss << "cells:\n";
    for (unsigned int y = 0; y < field.getHeight(); ++y) {
        for (unsigned int x = 0; x < field.getWidth(); ++x) {
            const Cell& cell = field.getCell(x, y);
            CellSerializer cellSerializer(cell);
            oss << cellSerializer.serialize();
            if (x < field.getWidth() - 1) oss << "|";
        }
        oss << "\n";
    }
    
    return oss.str();
}
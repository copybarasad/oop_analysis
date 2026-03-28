#ifndef CELLSERIALIZER_H
#define CELLSERIALIZER_H

#include "ISerializer.h"
#include "../Cell.h"

class CellSerializer : public ISerializer<Cell> {
public:
    nlohmann::json serialize(const Cell& cell) const override;
    void deserialize(Cell& cell, const nlohmann::json& json) const override;
};

#endif


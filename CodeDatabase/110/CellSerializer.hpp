#ifndef CELL_SER_H
#define CELL_SER_H

#include <algorithm>

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Field/Cell/Cell.hpp"
#include "../EntitySerializer/EntitySerializer.hpp"

class CellSerializer : public Serializable {
    private:
    const Cell& cell;
    
    public:
    CellSerializer (const Cell& cell);
    
    std::string serialize () const override;
};

#endif
#ifndef CELL_DESER_H
#define CELL_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "../EntityDeserializer/EntityDeserializer.hpp"

class CellDeserializer : public BaseDeserializer {
    public:
    Cell deserialize(const std::string& data, unsigned int x, unsigned int y);
};

#endif
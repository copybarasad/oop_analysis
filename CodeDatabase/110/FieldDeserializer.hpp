#ifndef FIELD_DESER_H
#define FIELD_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "modules/Field/Field/Field.hpp"
#include "../CellDeserializer/CellDeserializer.hpp"

class FieldDeserializer : public BaseDeserializer {
    public:
    Field* deserialize(const std::string& data);

    private:
    void parseAndSetCells(std::istringstream& stream, Field* field, unsigned int width, unsigned int height);
};

#endif
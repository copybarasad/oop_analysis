#ifndef FIELD_SER_H
#define FIELD_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../CellSerializer/CellSerializer.hpp"
#include "../../../../../Field/Field/Field.hpp"

class FieldSerializer : public Serializable {
    private:
    const Field& field;
    
    public:
    FieldSerializer (const Field& field);
    
    std::string serialize () const override;
};

#endif
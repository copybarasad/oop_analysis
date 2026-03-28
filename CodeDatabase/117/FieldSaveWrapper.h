#ifndef FIELDWRAPPER_H
#define FIELDWRAPPER_H

#include "CellSaveWrapper.h"
#include "../Field/Field.h"

class FieldSaveWrapper : public ISaveLoadWrapper {
private:
    Field& field;
    
public:
    explicit FieldSaveWrapper(Field& fieldRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif
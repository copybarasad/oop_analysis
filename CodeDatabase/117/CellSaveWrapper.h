#ifndef CELLWRAPPER_H
#define CELLWRAPPER_H

#include "ISaveLoadWrapper.h"
#include "../Field/Cell.h"

class CellSaveWrapper : public ISaveLoadWrapper {
private:
    Cell& cell;
    
public:
    explicit CellSaveWrapper(Cell& cellRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif
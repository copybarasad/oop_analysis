#include "Cell.h"

Cell::Cell(CellType cellType) : cellType(cellType),slowdown(false){}

Cell::CellType Cell::getType() const {
    return cellType;
}

void Cell::setType(CellType type){
    cellType = type;
}

bool Cell::isSlowing() const{return slowdown;}

void Cell::setSlowing(){slowdown = true;};

nlohmann::json Cell::toJson() const {
        return {
            {"type", static_cast<int>(cellType)}
        };
    }

void Cell::fromJson(const nlohmann::json& j) {
        cellType = static_cast<CellType>(j["type"]);
    }
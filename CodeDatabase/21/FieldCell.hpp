#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>
#include "CellState.hpp"
#include "FileHandler.hpp"
#include "SaveData.hpp"

class FieldCell {
private:
    CellState state;
    int index;
    int X;
    int Y;
public:
    FieldCell(
        int index,
        int X,
        int Y,
        std::shared_ptr<IState> constState = nullptr,
        std::shared_ptr<IState> temporaryState = nullptr
    );
    int getIndex() const;
    char getCellSymbol();
    CellSaveData getCellSaveData();

    void setCellSaveData(CellSaveData data);
    void setIndex(int index);
    float getDistance(const FieldCell& oth) const;
    std::pair<int, int> getCoord() const;
    CellState& returnCellState();
};

#include "FieldCell.hpp"


FieldCell::FieldCell(int index, int X, int Y,
    std::shared_ptr<IState> constState,
    std::shared_ptr<IState> temporaryState) {
    this->index = index;
    this->X = X;
    this->Y = Y;
    this->state.setConstState(move(constState));
    this->state.setTemporaryState(move(temporaryState));
}


int FieldCell::getIndex() const {
    return index;
}


std::pair<int, int> FieldCell::getCoord() const {
    return {X,Y};
}


void FieldCell::setIndex(int index) {
    this->index = index;
}


float FieldCell::getDistance(const FieldCell& oth) const {
    auto othCoord = oth.getCoord();
    int dX = std::abs(othCoord.first - X);
    int dY = std::abs(othCoord.second - Y);
    float distance = pow((std::pow(dX, 2) + std::pow(dY, 2)), 0.5);
    return distance;
}


char FieldCell::getCellSymbol() {
    char res = '_';
    if (!state.getAvaible()) {
        res = '#';
    }
    if (state.haveSpecificState()) {
        res = state.getCellSymbol();
    }
    return res;
}


CellState& FieldCell::returnCellState() {
    return state;
}

CellSaveData FieldCell::getCellSaveData() {
    CellSaveData data;
    data.stateData = state.getStateCellSaveData();
    data.index = index;
    data.X = X;
    data.Y = Y;
    return data;
}

void FieldCell::setCellSaveData(CellSaveData data) {
    this->index = data.index;
    this->X = data.X;
    this->Y = data.Y;
    this->state.setStateCellSaveData(data.stateData);
}
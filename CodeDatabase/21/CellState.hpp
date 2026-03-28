#pragma once
#include "./states/IState.hpp"
#include <memory>
#include "SaveData.hpp"

class CellState {
private:
    std::shared_ptr<IState> constState;
    std::shared_ptr<IState> temporaryState;
    bool avaible;
public:
    CellState();
    CellState(std::shared_ptr<IState> constState, std::shared_ptr<IState> temporaryState);
    char getCellSymbol();
    bool haveSpecificState() const;
    void setConstState(std::shared_ptr<IState> constState);
    void setTemporaryState(std::shared_ptr<IState> temporaryState);
    bool getAvaible() const;
    void setAvaible(bool newAvaible);
    int getStateDamage() const;

    CellStateSaveData getStateCellSaveData();
    void setStateCellSaveData(CellStateSaveData data);
};

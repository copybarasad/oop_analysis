#include "TrapSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool TrapSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    int up = userIndex - GlobalGameConfig::fieldWidth;
    int down = userIndex + GlobalGameConfig::fieldWidth;
    int left = userIndex - 1;
    int right = userIndex + 1;
    int currentTrapDamage = this->trapDamage * trapLevel * powerOfSpell + power * this->trapDamage;
    CellState& upCellState = ctx.cells[up].returnCellState();
    CellState& downCellState = ctx.cells[down].returnCellState();
    CellState& leftCellState = ctx.cells[left].returnCellState();
    CellState& rightCellState = ctx.cells[right].returnCellState();
    std::shared_ptr<IState> trapState = std::make_shared<TrapEffect>('o', currentTrapDamage, 1);
    if (upCellState.getAvaible() && !upCellState.haveSpecificState()) {
        upCellState.setConstState(move(trapState));
        return true;
    }
    else if (downCellState.getAvaible() && !downCellState.haveSpecificState()) {
        downCellState.setConstState(move(trapState));
        return true;
    }
    else if (leftCellState.getAvaible() && !leftCellState.haveSpecificState()) {
        leftCellState.setConstState(move(trapState));
        return true;
    }
    else if (rightCellState.getAvaible() && !rightCellState.haveSpecificState()) {
        rightCellState.setConstState(move(trapState));
        return true;
    }
    return false;
}

SpellSaveData TrapSpell::getSpellSaveData() {
    SpellSaveData data;
    data.countOfItem = this->countOfItem;
    data.powerOfSpell = this->powerOfSpell;
    data.baseDamage = 0;
    data.baseDistance = 0;
    data.trapDamage = this->trapDamage;
    data.trapLevel = this->trapLevel;
    return data;
}
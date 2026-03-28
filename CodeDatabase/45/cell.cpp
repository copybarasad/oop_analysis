#include "cell.hpp"
#include <iostream>

Cell::Cell() 
    : state(CellState::EMPTY), entity(nullptr) {
}

void Cell::transferEntityTo(Cell& other) {
    if (other.getState() == CellState::EMPTY || other.getState() == CellState::TRAP) {
        bool trapped = other.getState() == CellState::TRAP;
        other.setEntity(entity, state);

        if (trapped) {
            other.applyDamage(100);
        }
       
        entity = nullptr;
        state = CellState::EMPTY;
    }
}

CellState Cell::getState() const {
    return state;
}

void Cell::setEntity(Entity* newEntity, CellState newState) {
    entity = newEntity;
    state = newState;
}

void Cell::removeEntity() {
    if (state == CellState::MONSTER && entity != nullptr) {
        delete entity;
    }
    entity = nullptr;
    state = CellState::EMPTY;
}

void Cell::applyDamage(int damageAmount) {
    if (entity != nullptr) {
        entity->takeDamage(damageAmount);
        if (entity->getHp() <= 0) {
            removeEntity();
        }
    }
}

int Cell::getEntityDamage() const {
    if (entity != nullptr) {
        return entity->getDamage();
    }
    return 0;
}

bool Cell::getEntityCanMove() const {
    if (entity != nullptr) {
        return entity->getCanMove();
    }
    return false;
}

void Cell::setEntityCanMove(bool can) {
    if (entity != nullptr) {
        entity->setCanMove(can);
    }
}

void Cell::setWall(bool wall) {
    if (wall && entity == nullptr) {
        state = CellState::WALL;
    }
    else if (!wall && state == CellState::WALL) {
        state = CellState::EMPTY;
    }
}

void Cell::setTrap(bool trap) {
    if (trap && entity == nullptr) {
        state = CellState::TRAP;
    }
    else if (!trap && state == CellState::TRAP) {
        state = CellState::EMPTY;
    }
}

int Cell::getEntityHp() const {
    if (entity != nullptr) {
        return entity->getHp();
    }
    return 0;
}

void Cell::entityMakeStep(Map* map, int x, int y) {
    if (entity != nullptr) {
        entity->step(map, x, y);
    }
}

std::string Cell::getCellStatus() const {
    if (entity != nullptr) {
        return entity->getStatus();
    } else {
        switch (state) {
            case CellState::EMPTY:
                return "Empty";
                break;
            case CellState::WALL:
                return "Wall";
                break;
            case CellState::TRAP:
                return "Trap";
                break;
            default:
                return "ERROR";
        }
    }
}
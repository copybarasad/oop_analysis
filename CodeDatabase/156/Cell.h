//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_CELL_H
#define LABA1_CELL_H

#include "../Entities/Entity.h"
#include "CellType.h"
#include "../Exceptions/Notifications/NoEntityInCellNotification.h"
#include "CellEvents/CellEvent.h"
#include "CellEvents/SlowingCellEvent.h"
#include <memory>
#include <utility>
#include "CellEvents/TrapCellEvent.h"
#include "../Exceptions/UnexpectedBehaviorException.h"

class SpellFactory;


class Cell {
public:
    Cell(CellType type) : type(type){
        entityInCell = nullptr;
        event = nullptr;
    }

    Cell(CellType type, std::shared_ptr<Entity> entityInCell, std::unique_ptr<CellEvent> event) : type(type),
                                                                                         entityInCell(entityInCell),
                                                                                         event(std::move(event)) {}


    Cell(Cell&& other) noexcept : type(other.type){
        this->entityInCell = other.entityInCell;
        other.entityInCell = nullptr;
        this->event = std::move(other.event);
    }

    Cell& operator=(Cell&& other) noexcept {
        if (this != &other){
            this->type = other.type;
            this->entityInCell = other.entityInCell;
            other.entityInCell = nullptr;
            this->event = std::move(other.event);
        }
        return *this;
    }

    Cell(const Cell& other): type(other.type){
        if (other.hasCellEvent()){
            this->event = std::move(other.event->clone());
        }else{
            this->event = nullptr;
        }
        if (other.hasEntityInCell()){
            this->entityInCell = std::move(other.entityInCell->clone());
        }else{
            this->entityInCell = nullptr;
        }
    }

    Cell& operator=(const Cell& other){
        if (this != &other){
            this->type = other.type;
            if (other.hasCellEvent()){
                this->event = std::move(other.event->clone());
            }else{
                this->event = nullptr;
            }
            if (other.hasEntityInCell()){
                this->entityInCell = std::move(other.entityInCell->clone());
            }else{
                this->entityInCell = nullptr;
            }
        }
        return *this;
    }






    virtual void addEntityInCell(std::shared_ptr<Entity> entity) noexcept final;

    virtual void clearCell() noexcept final;

    virtual std::shared_ptr<Entity> getEntityInCell() noexcept final;

    virtual void setCellEvent(std::unique_ptr<CellEvent> cellEvent) final;

    virtual void impactOnCreatureByCellEvent() final; //к этому моменту creature уже должно быть перемещено в эту клетку

    virtual void damageEntityInCell(int damage) final;



    virtual bool hasEntityInCell() const noexcept final;

    virtual CellType getCellType() const noexcept final;

    virtual EntityType getEntityInCellType() const final;

    virtual CellEventType getCellEventType() const final;

    virtual bool hasCellEvent() const final;

    std::string serialize();
    static Cell deserialize(std::map<std::string, std::string>& json);

protected:
    CellType type;
    std::shared_ptr<Entity> entityInCell;
    mutable std::unique_ptr<CellEvent> event;
};


#endif //LABA1_CELL_H

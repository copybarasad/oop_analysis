//
// Created by Artem on 24.09.2025.
//

#include "Cell.h"
#include "../GameSetup/Utils/JsonParser.h"
#include "../Entities/Creatures/Player.h"
#include "../Entities/Buildings/EnemySpawnerBuilding.h"
#include "../Entities/Buildings/EnemyDefenceTower.h"

CellType Cell::getCellType() const noexcept{
    return type;
}

bool Cell::hasEntityInCell() const noexcept{
    return this->entityInCell != nullptr;
}

void Cell::addEntityInCell(std::shared_ptr<Entity> entity) noexcept {
    this->entityInCell = entity;
}

void Cell::clearCell() noexcept {
    this->entityInCell = nullptr;
}

void Cell::damageEntityInCell(int damage) {
    if (this->hasEntityInCell()){
        this->entityInCell->changeHealthPoints((-1) * damage);
        return;
    }
    throw NoEntityInCellNotification("В клетке нету сущности");
}

EntityType Cell::getEntityInCellType() const {
    if (this->hasEntityInCell()){
        return this->entityInCell->getType();
    }
    throw NoEntityInCellNotification("В клетке нету сущности");
}

std::shared_ptr<Entity> Cell::getEntityInCell() noexcept {
    return this->entityInCell;
}


bool Cell::hasCellEvent() const {
    return this->event.get() != nullptr;
}

void Cell::impactOnCreatureByCellEvent() {
    if (!this->hasCellEvent()){
        throw UnexpectedBehaviorException("Для вызова impactOnCreatureByCellEvent необходимо наличие cellEvent в клетке");
    }
    if (!this->hasEntityInCell()){
        throw UnexpectedBehaviorException("На момент вызова impactOnCreatureByCellEvent сущность уже должна быть перемещена в клетку");
    }
    if (this->getEntityInCellType() != EntityType::EnemyEnt && this->getEntityInCellType() != EntityType::PlayerEnt && this->getEntityInCellType() != EntityType::Ally){
        throw UnexpectedBehaviorException("К клетке должен быть наследник класса Creature");
    }
    Creature* creature = dynamic_cast<Creature*>(this->entityInCell.get());
    this->event.get()->impactOnCreatureInCell(*creature);//Вот он собсна ООП во плоти, стока по смыслу эквивалент нижнему(уже нет, удалил)! Это писал я, а не нейронка блеат!!!!! Свитч писать даже бы не пришлось если бы не удаление ловушек после получения урона
    switch (this->event.get()->getType()) {
        case CellEventType::SlowingEvent:
            break;
        case CellEventType::TrapEvent:
            this->event.reset(); //удаление ловушки после применения
            break;
    }
}

void Cell::setCellEvent(std::unique_ptr<CellEvent> cellEvent) {
    this->event = std::move(cellEvent);
}

std::string Cell::serialize() {
    std::string res  = "{";
    res += "type:";
    switch (this->type) {
        case CellType::Default:
            res += "Default,";
            break;
        case CellType::Impassable:
            res += "Impassable,";
            break;
    }

    res += "event:{";

    if (this->event != nullptr){
        res += "eventType:";
        switch (this->event.get()->getType()) {
            case CellEventType::SlowingEvent:
                res += "SlowingEvent,";
                break;
            case CellEventType::TrapEvent:
                res += "TrapEvent,";
                break;
        }
        res += "eventValue:";
        res += this->event->serialize() + "},";
    }else{
        res += "},";
    }

    res += "entityInCell:{";
    if (this->entityInCell != nullptr && this->entityInCell->getType() != EntityType::PlayerEnt){
        res += "entityType:" + JsonParser::enumEntityTypeToStringType(this->entityInCell->getType()) + ",";
        res += "entityValue:";
        res += entityInCell->serialize() +"}";
    }else{
        res += "}";
    }
    res += "}";

    return res;
}

const std::map<std::string, CellType> stringTypeToCellTypeEnum = {
        { "Impassable", CellType::Impassable},
        {"Default", CellType::Default}
};
const std::map<std::string, CellEventType> stringTypeToCellEventTypeEnum = {
        { "SlowingEvent", CellEventType::SlowingEvent},
        {"TrapEvent", CellEventType::TrapEvent}
};

Cell Cell::deserialize(std::map<std::string, std::string>& json) {
    CellType type = stringTypeToCellTypeEnum.at(json.at("type"));
    Entity* entity = nullptr;
    CellEvent* event = nullptr;
    if (json.at("event") != "{}") {
        std::map<std::string, std::string> eventMap = JsonParser::parseJsonWithNestedObj(json.at("event"));
        CellEventType eventType = stringTypeToCellEventTypeEnum.at(eventMap.at("eventType"));
        switch (eventType) {
            case CellEventType::SlowingEvent:
                event = SlowingCellEvent::deserialize();
                break;
            case CellEventType::TrapEvent:
                event = TrapCellEvent::deserialize( JsonParser::parseJsonWithNestedObj(eventMap.at("eventValue")));
                break;
        }
    }

    if (json.at("entityInCell") != "{}"){
        std::map<std::string, std::string> entityMap = JsonParser::parseJsonWithNestedObj(json.at("entityInCell"));
        EntityType entType = JsonParser::stringTypeToEnumEntityType(entityMap.at("entityType"));

        std::map<std::string, std::string> entityValueMap = JsonParser::parseJsonWithNestedObj(entityMap.at("entityValue"));
        switch (entType) {
            case EntityType::PlayerEnt:
                break;
            case EntityType::EnemyEnt:
            case EntityType::Ally:
                entity = CompControlledCreature::deserialize(entityValueMap);
                break;
            case EntityType::EnemySpawnerBuildingEnt:
                entity = EnemySpawnerBuilding::deserialize(entityValueMap);
                break;
            case EntityType::EnemyDefenceTower:
                entity = EnemyDefenceTower::deserialize(entityValueMap);
                break;
        }
    }
    return Cell(type, std::shared_ptr<Entity>(entity),std::unique_ptr<CellEvent>(event));
}

CellEventType Cell::getCellEventType() const {
    return this->event->getType();
}






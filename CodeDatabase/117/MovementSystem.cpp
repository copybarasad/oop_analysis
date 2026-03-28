#include "MovementSystem.h"
#include "../LogSystem/MoveEvent.h"
#include "../LogSystem/Notifier.h"

bool MovementSystem::move(Character& object, std::pair<int, int> point, Field& field){
    if (!field.canMoveTo(point)){
        return false;
    }

    if (field.isValidPos(object.getPos())){
        field.getCell(object.getPos()).setBusysness(false);
    }

    object.setPos(point);
    field.getCell(point).setBusysness(true);
    return true;
}

bool MovementSystem::step(Character& object, int dx, int dy, Field& field){
    if(!object.isAlive()){
        return false;
    }

    if (object.getCondition() == CharacterCondition::Slowed){
        object.setCondition(CharacterCondition::Normal);
        return false;
    }

    std::pair<int, int> oldPos = object.getPos();

    int newX = oldPos.first + dx;
    int newY = oldPos.second + dy;

    std::pair<int, int> newPos = {newX, newY};

    if(!field.canMoveTo(newPos)){
        return false;
    }

    if (field.getCell(newX, newY).getCellType() == CellType::Slowing){
        object.setCondition(CharacterCondition::Slowed);
    }

    if (this->move(object, newPos, field)){
        MoveEvent event = MoveEvent(object.getName(), oldPos, newPos);
        Notifier::getInstance().notify(event);
        return true;
    }
    return false;
}

void MovementSystem::stepToTar(Character& object, Character& target, Field& field){
    int tarX = target.getPos().first;
    int tarY = target.getPos().second;
    int objX = object.getPos().first;
    int objY = object.getPos().second;
    int diffX = tarX - objX;
    int diffY = tarY - objY; 

    int stepX = (diffX == 0) ? 0 : diffX / abs(diffX);
    int stepY = (diffY == 0) ? 0 : diffY / abs(diffY);
    
    if (object.getCondition() == CharacterCondition::Slowed){
        object.setCondition(CharacterCondition::Normal);
        return;
    }

    if(!this->step(object, stepX, stepY, field)){
        if(!this->step(object, 0, stepY, field)){
            this->step(object, stepX, 0, field);
        }
    }
}

void MovementSystem::deleteFromField(Character& object, Field& field){
    if(!field.isValidPos(object.getPos())){
        return;
    }
    field.getCell(object.getPos()).setBusysness(false);
    object.setPos(std::make_pair(0, 0));
}
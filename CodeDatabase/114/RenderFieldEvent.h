#ifndef RENDERFIELDEVENT_H
#define RENDERFIELDEVENT_H

#include "GameEvent.h"
#include "../../models/GameField.h"

class RenderFieldEvent : public GameEvent {
private:
    GameField* field;

public:
    RenderFieldEvent(GameField* gameField);
    
    std::string toString() const override;
    
    GameField* getField() const { return field; }
};

#endif


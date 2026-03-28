#include "freezy_system.h"


void Freezy_System::freezyObject(GameObject& object){
    object.setFrozen(2);
}

void Freezy_System::freezyReduceObject(GameObject& object){
    object.reduceFrozen();
}
bool Freezy_System::isFrozen(GameObject& object){
    if (object.getFrozen() > 0){
        return true;
    }
    return false;
}

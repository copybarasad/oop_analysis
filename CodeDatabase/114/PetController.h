#ifndef PETCONTROLLER_H
#define PETCONTROLLER_H

#include "interfaces/IPetField.h"

class PetController {
private:
    IPetField* field;
public:
    explicit PetController(IPetField* field) : field(field) {}
    void processPetTurn();
};

#endif




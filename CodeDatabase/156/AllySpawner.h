//
// Created by Artem on 23.10.2025.
//

#ifndef LABAOOP2_ALLYSPAWNER_H
#define LABAOOP2_ALLYSPAWNER_H
#include "../model/CompControlledCreatureModel.h"
#include "../Entities/Creatures/CompControlledCreature.h"
#include "../GameField/Field.h"
#include "../GameField/Cell.h"
#include "../Exceptions/Notifications/CellImpassableNotification.h"
#include "../Exceptions/CoordinateException.h"
#include "../Exceptions/SpawnEntityException.h"
class GameMasterBase;

class AllySpawner {
public:
    AllySpawner(const CompControlledCreatureModel &allyModel, Field &field, GameMasterBase &gameMaster) : allyModel(allyModel),
                                                                                                field(field),
                                                                                                gameMaster(
                                                                                                        gameMaster) {}

    void createAlly(int x, int y);

protected:
    const CompControlledCreatureModel allyModel;
    Field& field;
    GameMasterBase& gameMaster;
};


#endif //LABAOOP2_ALLYSPAWNER_H

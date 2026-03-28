#ifndef LABAOOP2_GAMERCONSOLEINPUTSPOTTER_H
#define LABAOOP2_GAMERCONSOLEINPUTSPOTTER_H

#include "../../../Model/InputKeysModel.h"
#include "../PlayerController.h"
#include "../../../Entities/Creatures/PlayerView.h"


class GamerConsoleInputSpotter {
public:
    GamerConsoleInputSpotter(const PlayerView &player, const std::shared_ptr<PlayerController> &controller,
                             const InputKeysModel &keysModel) : player(player), controller(controller), keysModel(keysModel) {}

    void playerMove(GameMasterBase& master);
    void waitingForContinueCommand(GameMasterBase& master);
    void upgradePlayer(GameMasterBase& master);

private:
    PlayerView player;
    std::shared_ptr<PlayerController> controller;
    InputKeysModel keysModel;

    void changeAttackType(GameMasterBase& master, bool* isChanged);
    void attackOnCoords(GameMasterBase& master);
    void useSpell(GameMasterBase& master);
};


#endif //LABAOOP2_GAMERCONSOLEINPUTSPOTTER_H
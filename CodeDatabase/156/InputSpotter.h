//
// Created by Artem on 26.12.2025.
//

#ifndef LABAOOP2_INPUTSPOTTER_H
#define LABAOOP2_INPUTSPOTTER_H

#include "../../../Model/InputKeysModel.h"
#include "../PlayerController.h"
#include "../../../Entities/Creatures/PlayerView.h"
#include "GamerConsoleInputSpotter.h"

class InputSpotter {
public:
    InputSpotter(const PlayerView &player,
                 const std::shared_ptr<PlayerController> &controller,
                 const InputKeysModel &keysModel)
            : spotter(player, controller, keysModel) {}

    template<typename GameType>
    void playerMove(GameType& game) {
        spotter.playerMove(game);
    }

    template<typename GameType>
    void waitingForContinueCommand(GameType& game) {
        spotter.waitingForContinueCommand(game);
    }

    template<typename GameType>
    void upgradePlayer(GameType& game) {
        spotter.upgradePlayer(game);
    }

private:
    GamerConsoleInputSpotter spotter;
};


#endif //LABAOOP2_INPUTSPOTTER_H

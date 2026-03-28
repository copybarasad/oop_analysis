#ifndef RPG_SHOOTCOMMAND_H
#define RPG_SHOOTCOMMAND_H

#include "Command.h"
#include "../Game.h"
#include "../Player.h"

class ShootCommand : public Command {
    std::shared_ptr<Game>& game;
public:
    explicit ShootCommand(std::shared_ptr<Game>& game) : game(game) {}

    std::pair<bool, std::string> execute() override {
        auto& player = game->getPlayer();
        auto mode = player.getMode();
        if (mode == PlayerModes::RANGE) {
            auto* target = game->getEntityManager().findEntity(player.getTarget());
            if (!target)
                return {true, ""};
            if (target->getTeam() == player.getTeam())
                return {false, "Нельзя стрелять по союзникам\n"};
            if (target->takeDamage(player.getDamage()))
                game->getGameField().getCell(player.getTarget()).entityDied(target->getPoints());
            return {true, ""};
        }
        if (mode == PlayerModes::SPELL) {
            auto* spell = player.getSpellsHand().getSelectedSpell();
            auto target = player.getTarget();
            bool status = spell->cast(target, &player, game->getGameField(), game->getEntityManager());
            if (status) {
                player.setMode(player.getPreviousMode());
                return {true, ""};
            }
        }
        return {false, ""};
    }
};

#endif
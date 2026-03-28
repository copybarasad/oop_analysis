#include "Command.hpp"
#include "GameWorld.hpp"
#include "GameContext.hpp"
#include "Actions.hpp"
#include "Player.hpp"

void MoveCommand::execute(GameWorld& world, GameContext& context) {
    Player& player = world.getPlayer();
    player.act(action::Move, world.getGrid(), world.getMainEnemy(), world, context, dx_, dy_);
}

void ChangeAttackTypeCommand::execute(GameWorld& world, GameContext& context) {
    Player& player = world.getPlayer();
    player.act(action::ChangeAttackType, world.getGrid(), world.getMainEnemy(), world, context, newType_);
}

void AttackCommand::execute(GameWorld& world, GameContext& context) {
    Player& player = world.getPlayer();
    player.act(action::Attack, world.getGrid(), world.getMainEnemy(), world, context);
}

void GiveASpellCommand::execute(GameWorld& world, GameContext& context) {
    Player& player = world.getPlayer();
    player.act(action::GiveASpell, world.getGrid(), world.getMainEnemy(), world, context);
}

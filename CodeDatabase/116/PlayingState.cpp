#include "PlayingState.h"
#include "GameContext.h"

void PlayingState::handleInput(GameContext& context) {
    if(!context.isGameRunning()){
        context.initializeNewLevel();
        context.setGameRunning(true);
    }
    update(context);
    auto& inpHandler = context.getInputHandler();
    bool gameRunning = true;
    inpHandler.processInput(context.getController(),context.getStatsRenderer(),gameRunning);
    if (inpHandler.getTookTurn()) {
        context.getController().getWorld().getPlayer().updateCoins(3);
        context.getController().moveAlly();
        context.getController().moveEnemies();
        context.getController().FortressAttack();
        context.getController().towerSpawn();
    }
    inpHandler.waitForEnter();
    if(!gameRunning){
        context.transitionTo("Pause");
    }
}

void PlayingState::render(GameContext& context) {
    system("clear");
    auto& field = context.getController().getWorld().getField();
    context.getFieldRenderer().printFieldWithColors(field);
    context.getStatsRenderer().printPlayerStats(context.getController());
    context.getStatsRenderer().printGameInstructions();
}

void PlayingState::update(GameContext& context) {
    auto& player = context.getController().getWorld().getPlayer();
    auto& tower = context.getController().getWorld().getTower();
    if(!player.isAlive()){
        context.transitionTo("GameOver");
    }
    if(!tower.isAlive()){
        std::cout << "Вражеская башня уничтожена! Уровень пройден" << std::endl;
        context.completeCurrentLevel();
    }
}
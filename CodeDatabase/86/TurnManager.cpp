#include "TurnManager.hpp"
#include "GameWorld.hpp"
#include "GameContext.hpp"

TurnManager::TurnManager(GameWorld& world, GameContext& ctx,int startTurn)
        : world_(world), ctx_(ctx),turnCount_(startTurn) {}

TurnStatus TurnManager::runOneTurn() {
    ctx_.logger_.header(turnCount_);
    
    if (!playerTurn()) {
        return TurnStatus::SaveAndExit;
    }

    if (!world_.getMainEnemy().isAlive()) return TurnStatus::PlayerWon;

    mainEnemyTurn();
    if (!world_.getPlayer().isAlive()) return TurnStatus::PlayerDied;

    enemiesTurn();
    if (!world_.getPlayer().isAlive()) return TurnStatus::PlayerDied;
    
    constructionTurn();


    ctx_.displayManager.showMap(world_);
    turnCount_++;
    
    return TurnStatus::Continue;
}

bool TurnManager::playerTurn() {
    auto& player = world_.getPlayer();
    if(playerSkip_){
        ctx_.logger_.info("Player is still slowed!\n");
        playerSkip_ = false;
        return true;
    }
    
    std::cout << "Actions: 0-Move, 1-Swap Atk, 2-Attack, 3-Spell, 9-SAVE/MENU\n";
    int choice;
    std::cin >> choice;

    if (choice == 9) return false; 

    action act = static_cast<action>(choice);
    bool slowed = !player.act(act, world_.getGrid(), world_.getMainEnemy(), world_, ctx_);
    playerSkip_ = slowed;
    return true;
}

void TurnManager::mainEnemyTurn() {
    auto& mainEnemy = world_.getMainEnemy();
    auto& player = world_.getPlayer();

    if (mainEnemy.distanceTo(player) <= 1) {
        ctx_.logger_.action("Main enemy attacks player!");
        player.takeDamage(mainEnemy.getDamage());
    } 
    else if (turnCount_ % 20 == 0 && world_.getConstruction() == nullptr) {
        auto construction = mainEnemy.putConstruction(mainEnemy.getPos(), world_.getGrid(), ctx_.logger_);
        world_.setConstruction(std::move(construction));
    }
    else {
        mainEnemy.chase(world_.getGrid(), player);
    }
}

void TurnManager::enemiesTurn() {
    for (auto& enemy : world_.getEnemies()) {
        enemy->chase(world_.getGrid(), world_.getPlayer());
    }
}

void TurnManager::constructionTurn() {
    auto tower = world_.getConstruction();
    if (!tower)
        return;

    if (turnCount_ % tower->getRate() == 0) {
        auto newEnemy = tower->spawnNewEnemy();
	if(newEnemy !=nullptr){

	    world_.addEnemy(std::move(newEnemy));
	    ctx_.logger_.info("A new enemy has spawned!");
	}
    }
}

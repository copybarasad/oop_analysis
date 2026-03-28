#include "GameCycleService.hpp"

GameCycleService::GameCycleService (std::map<size_t, GameLevel*> levels, std::map<size_t, std::vector<Item*>> boosts, size_t currentLevel) :
    levels(levels), boosts(boosts), currentLevel(currentLevel), level(levels[currentLevel]), boost(0) {}

GameCycleService::GameState GameCycleService::runTurn () {
    level->updateState();
    return getState();
}

GameCycleService::GameState GameCycleService::getState () {
    GameState result(currentLevel, level->getField(), level->getHand(), level->getEntityManager(), level->getSpellStore());
    if (level->isLevelCompleted()) {
        if (level->isPlayerAlive()) {
            result.progress = GameProgress::LEVEL_COMPLETED;
            return result;
        }
        result.progress = GameProgress::PLAYER_DEAD;
        return result;
    }
    return result;
}

bool GameCycleService::isPlayerTurn () {
    return level->isWaitingForPlayerInput();
}

void GameCycleService::setPlayerAction (const std::string& action, ChangeableFields* changeableFields) {
    level->queuePlayerAction(action, changeableFields);
}

bool GameCycleService::allLevelsDone () {
    return getState().progress == GameProgress::LEVEL_COMPLETED && currentLevel == levels.size()-1;
}

void GameCycleService::furtherLevel () {
    if (allLevelsDone()) {
        return;
    }
    transferPlayer();
    level = levels[++currentLevel];
    produceBoost(boost);
    updatePlayersCards();
}

void GameCycleService::setBoost (size_t choice) { boost = choice; }

void GameCycleService::produceBoost (size_t choice) {
    switch(choice) {
        case 1: {
            Weapon* sword = dynamic_cast<Weapon*>(boosts[currentLevel][0]);
            levels[currentLevel]->getEntityManager().getTracker()[0]->getEntityAs<Player>().setWeapon(sword);
            break;
        }
        case 2: {
            Weapon* bow = dynamic_cast<Weapon*>(boosts[currentLevel][1]);
            levels[currentLevel]->getEntityManager().getTracker()[0]->getEntityAs<Player>().setWeapon(bow);
            break;
        }
        case 3: {
            Armor* armor = dynamic_cast<Armor*>(boosts[currentLevel][2]);
            levels[currentLevel]->getEntityManager().getTracker()[0]->getEntityAs<Player>().setArmor(armor);
            break;
        }
        default:
        return;
    }
}

void GameCycleService::transferPlayer () {
    Player& player = levels[currentLevel]->getEntityManager().getTracker()[0]->getEntityAs<Player>();
    player.setCurrentHealth(player.getMaxHealth());
    levels[currentLevel+1]->getEntityManager().getTracker()[0]->setEntity(player);
}

void GameCycleService::updatePlayersCards () {
    std::random_device rd;
    std::mt19937 gen(rd());

    SpellHand& hand = levels[currentLevel]->getHand();
    for (int i = 0; i < (int)hand.getSpellNumber()/2; i++) {
        size_t index = gen() % hand.getSpellNumber();
        hand.removeSpell(hand.getSpellName(index));
    }
}
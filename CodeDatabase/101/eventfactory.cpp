#include "eventfactory.h"
#include "gamestartedevent.h"
#include "levelstartedevent.h"
#include "playerattackedevent.h"
#include "playerdamagedevent.h"
#include "playermovedevent.h"
#include "spellcastevent.h"
#include "enemydiedevent.h"
#include "moneychangedevent.h"
#include "cardreceivedevent.h"
#include "playerhealedevent.h"
#include "enemydamageevent.h"
#include "enemyattackedevent.h"
#include "towerdamageevent.h"
#include "towerdestroyedevent.h"
#include "levelcompletedevent.h"
#include "scorechangedevent.h"

std::shared_ptr<GameEvent> EventFactory::createPlayerMoved(int fromX, int fromY, int toX, int toY) {
    return std::make_shared<PlayerMovedEvent>(fromX, fromY, toX, toY);
}

std::shared_ptr<GameEvent> EventFactory::createPlayerAttacked(const std::string& target, int damage, int x, int y) {
    return std::make_shared<PlayerAttackedEvent>(target, damage, x, y);
}

std::shared_ptr<GameEvent> EventFactory::createPlayerDamaged(int damage, const std::string& source) {
    return std::make_shared<PlayerDamagedEvent>(damage, source);
}

std::shared_ptr<GameEvent> EventFactory::createSpellCast(const std::string& spellName, int cost, int x, int y) {
    return std::make_shared<SpellCastEvent>(spellName, cost, x, y);
}

std::shared_ptr<GameEvent> EventFactory::createEnemyDied(const std::string& enemyType, int x, int y) {
    return std::make_shared<EnemyDiedEvent>(enemyType, x, y);
}

std::shared_ptr<GameEvent> EventFactory::createMoneyChanged(int amount, int newBalance) {
    return std::make_shared<MoneyChangedEvent>(amount, newBalance);
}

std::shared_ptr<GameEvent> EventFactory::createCardReceived(const std::string& cardName) {
    return std::make_shared<CardReceivedEvent>(cardName);
}


std::shared_ptr<GameEvent> EventFactory::createGameStarted() {
    return std::make_shared<GameStartedEvent>();
}

std::shared_ptr<GameEvent> EventFactory::createLevelStarted(int level) {
    return std::make_shared<LevelStartedEvent>(level);
}

std::shared_ptr<GameEvent> EventFactory::createPlayerHealed(int amount) {
    return std::make_shared<PlayerHealedEvent>(amount);
}

std::shared_ptr<GameEvent> EventFactory::createEnemyDamaged(int damage) {
    return std::make_shared<EnemyDamageEvent>(damage);
}

std::shared_ptr<GameEvent> EventFactory::createEnemyAttacked(int damage) {
    return std::make_shared<EnemyAttackedEvent>(damage);
}

std::shared_ptr<GameEvent> EventFactory::createTowerDamaged(int damage) {
    return std::make_shared<TowerDamageEvent>(damage);
}

std::shared_ptr<GameEvent> EventFactory::createTowerDestroyed() {
    return std::make_shared<TowerDestroyedEvent>();
}

std::shared_ptr<GameEvent> EventFactory::createLevelCompleted() {
    return std::make_shared<LevelCompletedEvent>();
}

std::shared_ptr<GameEvent> EventFactory::createScoreChanged(int newScore) {
    return std::make_shared<ScoreChangedEvent>(newScore);
}

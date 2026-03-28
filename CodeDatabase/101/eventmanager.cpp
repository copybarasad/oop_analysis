#include "eventmanager.h"
#include "eventfactory.h"
#include <algorithm>


EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(std::shared_ptr<EventListener> listener) {
    listeners_.push_back(listener);
}

void EventManager::unsubscribe(std::shared_ptr<EventListener> listener) {
    auto it = std::remove_if(listeners_.begin(), listeners_.end(),
                             [&listener](const std::shared_ptr<EventListener>& item) {
                                 return item == listener;
                             });
    listeners_.erase(it, listeners_.end());
}

void EventManager::notify(std::shared_ptr<GameEvent> event) {
    for (auto& listener : listeners_) {
        listener->onEvent(event);
    }
}


void EventManager::notifyGameStarted() {
    auto event = EventFactory::createGameStarted();
    getInstance().notify(event);
}

void EventManager::notifyLevelStarted(int level) {
    auto event = EventFactory::createLevelStarted(level);
    getInstance().notify(event);
}

void EventManager::notifyPlayerMoved(int fromX, int fromY, int toX, int toY) {
    auto event = EventFactory::createPlayerMoved(fromX, fromY, toX, toY);
    getInstance().notify(event);
}

void EventManager::notifyPlayerAttacked(const std::string& target, int damage, int x, int y) {
    auto event = EventFactory::createPlayerAttacked(target, damage, x, y);
    getInstance().notify(event);
}

void EventManager::notifyPlayerDamaged(int damage, const std::string& source) {
    auto event = EventFactory::createPlayerDamaged(damage, source);
    getInstance().notify(event);
}

void EventManager::notifySpellCast(const std::string& spellName, int cost, int x, int y) {
    auto event = EventFactory::createSpellCast(spellName, cost, x, y);
    getInstance().notify(event);
}

void EventManager::notifyEnemyDied(const std::string& enemyType, int x, int y) {
    auto event = EventFactory::createEnemyDied(enemyType, x, y);
    getInstance().notify(event);
}

void EventManager::notifyMoneyChanged(int amount, int newBalance) {
    auto event = EventFactory::createMoneyChanged(amount, newBalance);
    getInstance().notify(event);
}

void EventManager::notifyCardReceived(const std::string& cardName) {
    auto event = EventFactory::createCardReceived(cardName);
    getInstance().notify(event);
}

void EventManager::notifyScoreChanged(int newScore) {
    auto event = EventFactory::createScoreChanged(newScore);
    getInstance().notify(event);
}

void EventManager::notifyPlayerHealed(int amount) {
    auto event = EventFactory::createPlayerHealed(amount);
    getInstance().notify(event);
}

void EventManager::notifyEnemyDamaged(int damage) {
    auto event = EventFactory::createEnemyDamaged(damage);
    getInstance().notify(event);
}

void EventManager::notifyEnemyAttacked(int damage) {
    auto event = EventFactory::createEnemyAttacked(damage);
    getInstance().notify(event);
}

void EventManager::notifyTowerDamaged(int damage) {
    auto event = EventFactory::createTowerDamaged(damage);
    getInstance().notify(event);
}

void EventManager::notifyTowerDestroyed() {
    auto event = EventFactory::createTowerDestroyed();
    getInstance().notify(event);
}

void EventManager::notifyLevelCompleted() {
    auto event = EventFactory::createLevelCompleted();
    getInstance().notify(event);
}

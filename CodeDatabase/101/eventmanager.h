#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <vector>
#include <memory>
#include "eventlistener.h"
#include "eventfactory.h"
#include "gameevent.h"
#include "Enum.h"


class EventManager {
private:
    static EventManager* instance_;
    std::vector<std::shared_ptr<EventListener>> listeners_;

    EventManager() = default;

public:
    static EventManager& getInstance();

    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    void subscribe(std::shared_ptr<EventListener> lstr);

    void unsubscribe(std::shared_ptr<EventListener> lstr);

    void notify(std::shared_ptr<GameEvent> event);

    static void notifyGameStarted();
    static void notifyLevelStarted(int level);
    static void notifyPlayerMoved(int fromX, int fromY, int toX, int toY);
    static void notifyPlayerAttacked(const std::string& target, int damage, int x, int y);
    static void notifyPlayerDamaged(int damage, const std::string& source);
    static void notifySpellCast(const std::string& spellName, int cost, int x, int y);
    static void notifyEnemyDied(const std::string& enemyType, int x, int y);
    static void notifyMoneyChanged(int amount, int newBalance);
    static void notifyCardReceived(const std::string& cardName);
    static void notifyScoreChanged(int newScore);

    static void notifyPlayerHealed(int amount);
    static void notifyEnemyDamaged(int damage);
    static void notifyEnemyAttacked(int damage);
    static void notifyTowerDamaged(int damage);
    static void notifyTowerDestroyed();
    static void notifyLevelCompleted();
};


#endif // EVENTMANAGER_H

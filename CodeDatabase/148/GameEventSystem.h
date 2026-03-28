#ifndef GAMEEVENTSYSTEM_H
#define GAMEEVENTSYSTEM_H

#include "IGameLogger.h"
#include "GameEvent.h"
#include <vector>
#include <memory>
#include <algorithm>  // ← ДОБАВИТЬ!

/**
 * @file GameEventSystem.h
 * @brief Центральная система управления событиями
 */

/**
 * @class GameEventSystem
 * @brief Subject в паттерне Observer
 * 
 * Управляет подписчиками (логгерами) и рассылает им события.
 * Синглтон для глобального доступа.
 */
class GameEventSystem {
private:
    std::vector<IGameLogger*> loggers;  ///< Подписчики (не владеет!)
    
    // Singleton
    GameEventSystem() = default;
    GameEventSystem(const GameEventSystem&) = delete;
    GameEventSystem& operator=(const GameEventSystem&) = delete;
    
public:
    /**
     * @brief Получить единственный экземпляр
     */
    static GameEventSystem& getInstance() {
        static GameEventSystem instance;
        return instance;
    }
    
    /**
     * @brief Подписать логгер на события
     * @param logger Указатель на логгер (не владеет!)
     */
    void subscribe(IGameLogger* logger) {
        if (logger) {
            loggers.push_back(logger);
        }
    }
    
    /**
     * @brief Отписать логгер
     * @param logger Указатель на логгер
     */
    void unsubscribe(IGameLogger* logger) {
        auto it = std::remove(loggers.begin(), loggers.end(), logger);
        loggers.erase(it, loggers.end());
    }
    
    /**
     * @brief Уведомить всех подписчиков о событии
     * @param event Игровое событие
     */
    void notifyEvent(const GameEvent& event) {
        for (auto* logger : loggers) {
            logger->logEvent(event);
        }
    }
    
    /**
     * @brief Очистить всех подписчиков
     */
    void clearSubscribers() {
        loggers.clear();
    }
    
    /**
     * @brief Получить количество подписчиков
     */
    size_t getSubscriberCount() const {
        return loggers.size();
    }
};

#endif // GAMEEVENTSYSTEM_H



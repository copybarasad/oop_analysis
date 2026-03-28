#pragma once
#include "IGameLogger.h"
#include "GameEvent.h"
#include <vector>
#include <memory>

// Класс для уведомления логгеров о событиях
// Игровые сущности используют этот класс для информирования о событиях
class GameEventNotifier {
private:
    std::vector<std::unique_ptr<IGameLogger>> loggers_;
    
public:
    // Добавляет логгер
    void addLogger(std::unique_ptr<IGameLogger> logger) {
        loggers_.push_back(std::move(logger));
    }
    
    // Уведомляет все логгеры о событии
    void notify(const GameEvent& event) {
        for (auto& logger : loggers_) {
            if (logger) {
                logger->logEvent(event);
            }
        }
    }
    
    // Закрывает все логгеры
    void close() {
        for (auto& logger : loggers_) {
            if (logger) {
                logger->close();
            }
        }
    }
};


#pragma once
#include "GameEvent.h"

// Интерфейс для логирования игровых событий
class IGameLogger {
public:
    virtual ~IGameLogger() = default;
    
    // Логирует игровое событие
    virtual void logEvent(const GameEvent& event) = 0;
    
    // Закрывает логгер (для файловых логгеров)
    virtual void close() {}
};


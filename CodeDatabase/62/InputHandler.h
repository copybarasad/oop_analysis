#pragma once
#include "Event.h"
#include <vector>

// Интерфейс для считывания ввода и преобразования в команды
class InputHandler {
public:
    virtual ~InputHandler() = default;
    
    // Обрабатывает ввод и добавляет события в вектор
    // Возвращает true, если окно должно быть закрыто
    virtual bool processInput(std::vector<Event>& events) = 0;
};


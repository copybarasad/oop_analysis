#pragma once
#include "GameModel.h"
#include "Event.h"
#include "InputHandler.h"
#include <vector>

// Шаблонный класс управления игрой
// Параметр шаблона: класс для считывания и преобразования ввода
template<typename InputHandlerType>
class GameController {
private:
    GameModel& model_;
    std::vector<Event>& events_;
    InputHandlerType inputHandler_;

public:
    template<typename... Args>
    GameController(GameModel& model, std::vector<Event>& events, Args&&... args) 
        : model_(model)
        , events_(events)
        , inputHandler_(std::forward<Args>(args)...) {}
    
    bool processEvents(float dt) {
        // Получаем команды от InputHandler
        bool shouldClose = inputHandler_.processInput(events_);
        
        // Передаем события в GameModel для обработки
        model_.processEvents(events_);
        
        return shouldClose;
    }
};
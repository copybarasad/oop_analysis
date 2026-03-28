#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameModel.h"
#include "GameController.h"
#include "GameViewManager.h"
#include "SFMLInputHandler.h"
#include "SFMLRenderer.h"
#include "Event.h"
#include "GameEventNotifier.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include <vector>
#include <memory>

class Game {
private:
    sf::RenderWindow window_;
    sf::Clock clock;

    GameEventNotifier eventNotifier_;
    GameModel model;
    GameViewManager<SFMLRenderer> viewManager;
    GameController<SFMLInputHandler> controller;
    std::vector<Event> events_;

public:
    Game(int width, int height, const std::string& loggerType = "console", const std::string& logFile = "game.log");
    ~Game() = default;
    
    void run();
    
    std::vector<Event>& getEvents() { return events_; }
};
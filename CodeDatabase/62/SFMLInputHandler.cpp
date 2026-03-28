#include "SFMLInputHandler.h"

SFMLInputHandler::SFMLInputHandler(sf::RenderWindow& window, const std::string& configFile) 
        : window_(window) {
        std::cout << "Loading controls from " << configFile << std::endl;
        // Пытаемся загрузить конфигурацию из файла
        if (!config_.loadFromFile(configFile)) {
            // Если не удалось, используем значения по умолчанию
            std::cerr << "Using default controls" << std::endl;
            config_.setDefault();
        }
    }

bool SFMLInputHandler::processInput(std::vector<Event>& events) {
    bool shouldClose = false;
    
    while (std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            shouldClose = true;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                shouldClose = true;
            } else {
                // Получаем команду из конфигурации (включая новые команды)
                const CommandConfig* command = config_.getCommand(keyPressed->scancode);
                if (command != nullptr) {
                    Event event(command->eventType);
                    
                    // Добавляем аргументы
                    for (float arg : command->floatArgs) {
                        event.addFloatArg(arg);
                    }
                    for (int arg : command->intArgs) {
                        event.addIntArg(arg);
                    }
                    for (const std::string& arg : command->stringArgs) {
                        event.addStringArg(arg);
                    }
                    
                    events.push_back(event);
                }
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                Event mouseClickEvent(EventType::HandleMouseClick);
                events.push_back(mouseClickEvent);
            }
        }
    }
    
    return shouldClose;
}


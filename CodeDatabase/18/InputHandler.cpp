#include "InputHandler.h"
#include "Logger.h"

SfmlInputHandler::SfmlInputHandler() {
    setDefaultConfig();
}

void SfmlInputHandler::setWindow(sf::Window& win) {
    windowRef = &win;
}

void SfmlInputHandler::setDefaultConfig() {
    keyMap.clear();
    keyMap[sf::Keyboard::Key::W] = ActionType::MoveUp;
    keyMap[sf::Keyboard::Key::S] = ActionType::MoveDown;
    keyMap[sf::Keyboard::Key::A] = ActionType::MoveLeft;
    keyMap[sf::Keyboard::Key::D] = ActionType::MoveRight;
    keyMap[sf::Keyboard::Key::Num1] = ActionType::Spell1;
    keyMap[sf::Keyboard::Key::Num2] = ActionType::Spell2;
    keyMap[sf::Keyboard::Key::Num3] = ActionType::Spell3;
    keyMap[sf::Keyboard::Key::B] = ActionType::Shop;
    keyMap[sf::Keyboard::Key::Space] = ActionType::SwitchMode;
    keyMap[sf::Keyboard::Key::K] = ActionType::Save;
    keyMap[sf::Keyboard::Key::L] = ActionType::Load;
    keyMap[sf::Keyboard::Key::Escape] = ActionType::Exit;
    keyMap[sf::Keyboard::Key::N] = ActionType::NewGame;
    keyMap[sf::Keyboard::Key::C] = ActionType::Continue;
}

void SfmlInputHandler::loadConfig(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        EventManager::getInstance().notify(EventType::Error, "Config file not found, using default");
        return;
    }

    std::map<sf::Keyboard::Key, ActionType> tempMap;
    std::set<sf::Keyboard::Key> usedKeys;

    std::string keyStr, actionStr;
    while (ifs >> actionStr >> keyStr) {
        ActionType act = stringToAction(actionStr);
        sf::Keyboard::Key key = stringToKey(keyStr);

        if (act == ActionType::Unknown || key == sf::Keyboard::Key::Unknown) {
            EventManager::getInstance().notify(EventType::Error, "Invalid config line: " + actionStr + " " + keyStr);
            setDefaultConfig();
            return;
        }
        if (usedKeys.count(key)) {
            EventManager::getInstance().notify(EventType::Error, "Duplicate key in config: " + keyStr);
            setDefaultConfig();
            return;
        }

        tempMap[key] = act;
        usedKeys.insert(key);
    }

    keyMap = tempMap;
    EventManager::getInstance().notify(EventType::Info, "Custom controls loaded");
}

std::unique_ptr<Command> SfmlInputHandler::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        auto it = keyMap.find(keyEvent->code);
        if (it == keyMap.end()) return nullptr;

        ActionType act = it->second;

        if (act == ActionType::Spell1) selectedSpellIndex = 0;
        else if (act == ActionType::Spell2) selectedSpellIndex = 1;
        else if (act == ActionType::Spell3) selectedSpellIndex = 2;

        if (keyEvent->code == sf::Keyboard::Key::Num1) return std::make_unique<SelectUpgradeCommand>(1);
        if (keyEvent->code == sf::Keyboard::Key::Num2) return std::make_unique<SelectUpgradeCommand>(2);
        if (keyEvent->code == sf::Keyboard::Key::Num3) return std::make_unique<SelectUpgradeCommand>(3);

        switch (act) {
            case ActionType::MoveUp:    return std::make_unique<MoveCommand>(0, -1);
            case ActionType::MoveDown:  return std::make_unique<MoveCommand>(0, 1);
            case ActionType::MoveLeft:  return std::make_unique<MoveCommand>(-1, 0);
            case ActionType::MoveRight: return std::make_unique<MoveCommand>(1, 0);
            case ActionType::Shop:      return std::make_unique<GeneralActionCommand>("SHOP");
            case ActionType::SwitchMode:return std::make_unique<GeneralActionCommand>("SWITCH_MODE");
            case ActionType::Save:      return std::make_unique<GeneralActionCommand>("SAVE");
            case ActionType::Load:      return std::make_unique<MenuCommand>("LOAD");
            case ActionType::Exit:      return std::make_unique<MenuCommand>("EXIT");
            case ActionType::NewGame:   return std::make_unique<MenuCommand>("NEW");
            case ActionType::Continue:  return std::make_unique<MenuCommand>("CONTINUE");
            default: break;
        }

        if (keyEvent->code == sf::Keyboard::Key::Escape) return std::make_unique<MenuCommand>("MENU");
    }
    return nullptr;
}

std::unique_ptr<Command> SfmlInputHandler::handleMouse(const sf::Event& event, float tileSize) {
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left && windowRef && selectedSpellIndex != -1) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*windowRef);
            int tx = mousePos.x / static_cast<int>(tileSize);
            int ty = mousePos.y / static_cast<int>(tileSize);

            int idx = selectedSpellIndex;
            selectedSpellIndex = -1;
            return std::make_unique<UseSpellCommand>(tx, ty, idx);
        }
    }
    return nullptr;
}

sf::Keyboard::Key SfmlInputHandler::stringToKey(const std::string& s) {
    if (s == "W") return sf::Keyboard::Key::W;
    if (s == "A") return sf::Keyboard::Key::A;
    if (s == "S") return sf::Keyboard::Key::S;
    if (s == "D") return sf::Keyboard::Key::D;
    if (s == "SPACE") return sf::Keyboard::Key::Space;
    if (s == "ESC") return sf::Keyboard::Key::Escape;
    if (s == "1") return sf::Keyboard::Key::Num1;
    if (s == "2") return sf::Keyboard::Key::Num2;
    if (s == "3") return sf::Keyboard::Key::Num3;
    if (s == "B") return sf::Keyboard::Key::B;
    if (s == "K") return sf::Keyboard::Key::K;
    if (s == "L") return sf::Keyboard::Key::L;
    if (s == "N") return sf::Keyboard::Key::N;
    if (s == "C") return sf::Keyboard::Key::C;
    return sf::Keyboard::Key::Unknown;
}

ActionType SfmlInputHandler::stringToAction(const std::string& s) {
    if (s == "MOVE_UP") return ActionType::MoveUp;
    if (s == "MOVE_DOWN") return ActionType::MoveDown;
    if (s == "MOVE_LEFT") return ActionType::MoveLeft;
    if (s == "MOVE_RIGHT") return ActionType::MoveRight;
    if (s == "SPELL_1") return ActionType::Spell1;
    if (s == "SPELL_2") return ActionType::Spell2;
    if (s == "SPELL_3") return ActionType::Spell3;
    if (s == "SHOP") return ActionType::Shop;
    if (s == "SWITCH_MODE") return ActionType::SwitchMode;
    if (s == "SAVE") return ActionType::Save;
    if (s == "LOAD") return ActionType::Load;
    if (s == "EXIT") return ActionType::Exit;
    if (s == "NEW_GAME") return ActionType::NewGame;
    if (s == "CONTINUE") return ActionType::Continue;
    return ActionType::Unknown;
}
#include "InputController.h"
#include "../../models/Settings.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <cctype>

InputController::InputController(SettingsController* settingsController)
    : settingsController(settingsController) {}

bool InputController::matchesKey(char input, char expected) {
    return std::tolower(static_cast<unsigned char>(input)) ==
           std::tolower(static_cast<unsigned char>(expected));
}

const Settings& InputController::resolveSettings() const {
    static Settings defaultSettings = Settings::createDefault();
    if (!settingsController) {
        return defaultSettings;
    }
    return settingsController->getSettings();
}

std::string InputController::buildPrompt(const Settings& settings) const {
    std::ostringstream prompt;
    prompt << "\nКоманды: "
           << settings.moveUp << "/" << settings.moveLeft << "/" << settings.moveDown << "/" << settings.moveRight
           << " - движение, "
           << settings.switchCombatMode << " - смена режима боя, "
           << settings.attack << " - атака, "
           << settings.buySpell << " - купить заклинание, "
           << settings.castSpell << " - применить заклинание, "
           << settings.saveGame << " - сохранить, "
           << settings.loadGame << " - загрузить, "
           << settings.help << " - помощь, "
           << settings.exitGame << " - выход: ";
    return prompt.str();
}

CommandType InputController::mapKeyToCommand(char input, const Settings& settings) const {
    if (matchesKey(input, settings.moveUp)) {
        return CommandType::MOVE_UP;
    }
    if (matchesKey(input, settings.moveDown)) {
        return CommandType::MOVE_DOWN;
    }
    if (matchesKey(input, settings.moveLeft)) {
        return CommandType::MOVE_LEFT;
    }
    if (matchesKey(input, settings.moveRight)) {
        return CommandType::MOVE_RIGHT;
    }
    if (matchesKey(input, settings.switchCombatMode)) {
        return CommandType::SWITCH_COMBAT_MODE;
    }
    if (matchesKey(input, settings.attack)) {
        return CommandType::ATTACK;
    }
    if (matchesKey(input, settings.castSpell)) {
        return CommandType::CAST_SPELL;
    }
    if (matchesKey(input, settings.buySpell)) {
        return CommandType::PURCHASE_SPELL;
    }
    if (matchesKey(input, settings.saveGame)) {
        return CommandType::SAVE_GAME;
    }
    if (matchesKey(input, settings.loadGame)) {
        return CommandType::LOAD_GAME;
    }
    if (matchesKey(input, settings.help)) {
        return CommandType::HELP;
    }
    if (matchesKey(input, settings.exitGame)) {
        return CommandType::QUIT;
    }
    return CommandType::UNKNOWN;
}

CommandType InputController::readCommand() {
    const auto& settings = resolveSettings();
    PrintMessageEvent promptEvent(buildPrompt(settings));
    EventNotifier::getInstance().notify(promptEvent);
    
    char input;
    std::cin >> input;
    
    // Очистка буфера
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return mapKeyToCommand(input, settings);
}

std::string InputController::getInput(const std::string& prompt) {
    PrintMessageEvent promptEvent(prompt);
    EventNotifier::getInstance().notify(promptEvent);
    
    std::string input;
    std::cin >> input;
    
    // Очистка буфера ввода
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return input;
}

int InputController::getIntInput(const std::string& prompt) {
    int value;
    bool validInput = false;
    
    while (!validInput) {
        PrintMessageEvent promptEvent(prompt);
        EventNotifier::getInstance().notify(promptEvent);
        
        // Проверяем, что поток ввода в корректном состоянии
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        std::cin >> value;
        
        // Очистка буфера ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Проверяем, что чтение прошло успешно
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            PrintMessageEvent errorEvent("Некорректный ввод. Введите целое число.");
            EventNotifier::getInstance().notify(errorEvent);
            continue;
        }
        
        validInput = true;
    }
    
    return value;
}

void InputController::getIntPairInput(const std::string& prompt, int& x, int& y) {
    bool validInput = false;
    
    while (!validInput) {
        PrintMessageEvent promptEvent(prompt);
        EventNotifier::getInstance().notify(promptEvent);
        
        // Проверяем, что поток ввода в корректном состоянии
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        if (!(std::cin >> x >> y)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            PrintMessageEvent errorEvent("Некорректный ввод. Введите два целых числа через пробел.");
            EventNotifier::getInstance().notify(errorEvent);
            continue;
        }
        
        // Очистка буфера ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        validInput = true;
    }
}

void InputController::processPlayerUpgrade(bool& upgradeHealth, bool& upgradeDamage) {
    PrintMessageEvent headerEvent("\n=== ПРОКАЧКА ИГРОКА ===");
    EventNotifier::getInstance().notify(headerEvent);
    PrintMessageEvent infoEvent("Выберите, что улучшить на 10 единиц: (+50 HP положено автоматом)");
    EventNotifier::getInstance().notify(infoEvent);
    PrintMessageEvent option1Event("1 - Уровень здоровья (+10 к максимальному и текущему здоровью)");
    EventNotifier::getInstance().notify(option1Event);
    PrintMessageEvent option2Event("2 - Урон (+10 к урону ближнего и дальнего боя)");
    EventNotifier::getInstance().notify(option2Event);
    
    int choice;
    bool validInput = false;
    
    while (!validInput) {
        choice = getIntInput("Ваш выбор (1 или 2): ");
        
        if (choice == 1) {
            upgradeHealth = true;
            PrintMessageEvent successEvent("Выбрано улучшение здоровья на 10 единиц.");
            EventNotifier::getInstance().notify(successEvent);
            validInput = true;
        } else if (choice == 2) {
            upgradeDamage = true;
            PrintMessageEvent successEvent("Выбрано улучшение урона на 10 единиц.");
            EventNotifier::getInstance().notify(successEvent);
            validInput = true;
        } else {
            PrintMessageEvent errorEvent("Некорректный выбор. Введите 1 или 2.");
            EventNotifier::getInstance().notify(errorEvent);
        }
    }
}

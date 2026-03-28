#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "../../models/commands/Command.h"
#include "../SettingsController.h"
#include <string>

class InputController {
private:
    SettingsController* settingsController;
    
    const Settings& resolveSettings() const;
    std::string buildPrompt(const Settings& settings) const;
    static bool matchesKey(char input, char expected);
    CommandType mapKeyToCommand(char input, const Settings& settings) const;

public:
    InputController(SettingsController* settingsController = nullptr);
    ~InputController() = default;
    
    // Читает команду и возвращает её тип
    CommandType readCommand();
    
    // Вспомогательные методы
    void processPlayerUpgrade(bool& upgradeHealth, bool& upgradeDamage);
    std::string getInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
    void getIntPairInput(const std::string& prompt, int& x, int& y);
    
    void setSettingsController(SettingsController* controller) {
        settingsController = controller;
    }
};

#endif

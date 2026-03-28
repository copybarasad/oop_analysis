#ifndef RPG_CONSOLEINPUTMANAGER_H
#define RPG_CONSOLEINPUTMANAGER_H

#include "IInputManager.h"
#include <termios.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ConsoleInputManager : public IInputManager {
    termios originalTerminalSettings;

    void loadMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) override;

    void setDefaultMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) override;

    void loadGameControls(std::shared_ptr<Game>& game, bool& changeMode) override;

    void setDefaultGameControls(std::shared_ptr<Game>& game, bool& changeMode) override;
public:
    ConsoleInputManager();

    ~ConsoleInputManager() override;

    std::string setupMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) override;

    std::string setupGameControls(std::shared_ptr<Game>& game, bool& changeMode) override;

    std::string& getControls() override;

    Command* getCommand() override;
};

#endif
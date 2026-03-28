#ifndef RPG_INPUTMANAGER_H
#define RPG_INPUTMANAGER_H

#include <memory>
#include <unordered_map>
#include "Game.h"
#include "Commands/Command.h"

class IInputManager {
    virtual void loadMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) = 0;

    virtual void setDefaultMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) = 0;

    virtual void loadGameControls(std::shared_ptr<Game>& game, bool& changeMode) = 0;

    virtual void setDefaultGameControls(std::shared_ptr<Game>& game, bool& changeMode) = 0;
protected:
    std::unordered_map<char, std::shared_ptr<Command>> controls;
    std::string bindsInfo;
public:
    virtual ~IInputManager() = default;

    virtual std::string setupMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) = 0;

    virtual std::string setupGameControls(std::shared_ptr<Game>& game, bool& changeMode) = 0;

    virtual std::string& getControls() = 0;

    virtual Command* getCommand() = 0;
};

#endif
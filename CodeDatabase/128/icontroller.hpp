#pragma once
#include "gamemodel.hpp"
#include "enums.hpp"

class IController {
public:
    virtual ~IController() = default;

    virtual Game& getGame() = 0;
    virtual void SetState(GameStateType type) = 0;

    virtual bool LoadGame(const std::string& filename) = 0;
    virtual void SaveGame(const std::string& filename) = 0;
    virtual void Stop() = 0;
    virtual void StartNewGame() = 0;
};

#ifndef LOAD_S_H
#define LOAD_S_H

#include "../../SaveLoadException/ValidationException/ValidationException.hpp"
#include "modules/Services/GameCycleService/GameCycleService.hpp"

class LoadService {
    public:
    virtual ~LoadService () = default;

    virtual GameCycleService::GameState* load(const std::string& saveName) = 0;
    virtual bool saveExists(const std::string& saveName) const = 0;
};

#endif
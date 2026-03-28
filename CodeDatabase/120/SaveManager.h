#pragma once

#include "FileWriteException.h"
#include "json.hpp"
#include "Utils.h"
#include "GameWorld.h"

class SaveManager {
 public:
  bool SaveGame(const std::string& filename, GameWorld& world);
};

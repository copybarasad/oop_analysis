#pragma once

#include "GameLogic.h"
#include "SaveLoadException.h"
class ExceptionHandler {
  GameLogic* logic;

 public:
  explicit ExceptionHandler(GameLogic* logicPtr = nullptr);

  template <typename Func>
  bool Handle(Func&& func) {
    try {
      return func();
    } catch (const SaveLoadException& ex) {
      std::cerr << "Save/Load error: " << ex.what() << std::endl;
      if (logic) logic->events.Notify(GameMessageEvent{"Save/Load error: " + std::string(ex.what())});
      return false;
    } catch (const std::exception& ex) {
      std::cerr << "Unexpected error: " << ex.what() << std::endl;
      if (logic) logic->events.Notify(GameMessageEvent{"Unexpected error: " + std::string(ex.what())});
      return false;
    } catch (...) {
      std::cerr << "Unknown error occurred during save/load" << std::endl;
      if (logic) logic->events.Notify(GameMessageEvent{"Unknown error occurred during save/load"});
      return false;
    }
  }
};

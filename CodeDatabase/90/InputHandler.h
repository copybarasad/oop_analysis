#ifndef RPG_INPUT_HANDLER_H_
#define RPG_INPUT_HANDLER_H_

#include "Position.h"
#include <string>
#include <termios.h>
#include <unistd.h>

class Game;

class InputHandler {
private:
  Game &game;
  struct TargetingState {
    bool active = false;
    int radius = 0;
    Position cursor{0, 0};
    int areaW = 1;
    int areaH = 1;
  } targeting;
  std::string statusMessage;

public:
  InputHandler(Game &game) : game(game) {}
  void enableRawMode();
  void disableRawMode();
  char getChar();
  bool handleInput(char input);
  const TargetingState &getTargetingState() const { return targeting; }
  void clearTargeting() { targeting.active = false; }
  bool isTargetingActive() const { return targeting.active; }
  const std::string &getStatusMessage() const { return statusMessage; }
  void setStatusMessage(const std::string &msg) { statusMessage = msg; }
  void initSaving();
  void initLoading();
};

#endif // RPG_INPUT_HANDLER_H_

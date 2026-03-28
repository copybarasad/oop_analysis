#include <iostream>
#include <memory>
#include "gamecontroller.h"

int main()
{
  try
  {
    GameController game;
    game.start();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
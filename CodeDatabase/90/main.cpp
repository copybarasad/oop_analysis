#include "Game.h"
#include <iostream>

int main() {
  bool playAgain = true;
  while (playAgain) {
    try {
      Game game = Game();
      game.run();
      std::cout << "\nWould you like to restart (y/n)? ";
      char response;
      std::cin >> response;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      playAgain = (response == 'y' || response == 'Y');
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }
  std::cout << "Thanks for playing!" << std::endl;
  return 0;
}
#include "GameRenderer.h"

void ConsoleRenderer::render(const Game& game) {
  std::cout << "\n=== GAME ===" << std::endl;
  renderPlayerInfo(game.getPlayer());
  renderMap(game.getMap());
  std::cout << "============" << std::endl;
}

void ConsoleRenderer::renderPlayerInfo(const Player& player) {
  std::cout << "Health: " << player.getHealth() 
            << " | Score: " << player.getScore()
            << " | Mana: " << player.getMana() << "/" << player.getMaxMana();
  
  if (player.getMana() == 0) {
    std::cout << " [NO MANA]";
  }
  
  if (player.getInvisibilityTurns() > 0) {
    std::cout << " | INVISIBLE (" << player.getInvisibilityTurns() << " turns)";
  }

  if (player.getEnhancementPower() > 0) {
    std::cout << " | ENHANCE POWER: " << player.getEnhancementPower();
  }
  
  std::cout << std::endl;
  
  // Заклинания
  std::cout << "Spells: ";
  for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
    Spell* spell = player.getSpellHand().getSpell(i);
    std::cout << (i+1) << "." << spell->getName() 
              << "(" << spell->getManaCost() << " mana) ";
  }
  std::cout << std::endl;
}

void ConsoleRenderer::renderMap(const GameMap& map) {
  map.display();
}

void ConsoleRenderer::renderMenu(const std::vector<std::string>& options) {
  std::cout << "\n=== MENU ===" << std::endl;
  for (size_t i = 0; i < options.size(); ++i) {
    std::cout << (i+1) << ". " << options[i] << std::endl;
  }
  std::cout << "============" << std::endl;
}

void ConsoleRenderer::renderMessage(const std::string& message) {
  std::cout << message << std::endl;
}
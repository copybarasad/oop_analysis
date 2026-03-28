#include "Command.h"
#include "InputHandler.h"
#include <iostream>

void MoveCommand::execute(GameController& controller) {
    controller.processPlayerMove(direction);
}

std::string MoveCommand::toString() const {
    return "MoveCommand(" + std::to_string(direction.x) + "," + std::to_string(direction.y) + ")";
}

void AttackCommand::execute(GameController& controller) {
    controller.processPlayerAttack(direction);
}

std::string AttackCommand::toString() const {
    return "AttackCommand(" + std::to_string(direction.x) + "," + std::to_string(direction.y) + ")";
}

void AreaAttackCommand::execute(GameController& controller) {
    controller.processPlayerAreaAttack();
}

std::string AreaAttackCommand::toString() const {
    return "AreaAttackCommand";
}

void SwitchCombatCommand::execute(GameController& controller) {
    controller.processPlayerSwitchCombat();
}

std::string SwitchCombatCommand::toString() const {
    return "SwitchCombatCommand";
}

void UseSpellCommand::execute(GameController& controller) {
    controller.useSpell(spellIndex, target);
}

std::string UseSpellCommand::toString() const {
    return "UseSpellCommand(index=" + std::to_string(spellIndex) + 
           ", target=(" + std::to_string(target.x) + "," + std::to_string(target.y) + "))";
}

void BuySpellCommand::execute(GameController& controller) {
    controller.buyRandomSpell(30);
}

std::string BuySpellCommand::toString() const {
    return "BuySpellCommand";
}

void UpgradeSpellCommand::execute(GameController& controller) {
    controller.upgradeSpell(spellIndex);
}

std::string UpgradeSpellCommand::toString() const {
    return "UpgradeSpellCommand(index=" + std::to_string(spellIndex) + ")";
}

void ListSpellsCommand::execute(GameController& controller) {
    controller.getHand().listSpells(controller.getPlayer());
}

std::string ListSpellsCommand::toString() const {
    return "ListSpellsCommand";
}

void SaveGameCommand::execute(GameController& controller) {
    std::cout << "\n=== SAVE GAME ===" << std::endl;
    std::cout << "To save the game, use 'V' in the main game loop." << std::endl;
    std::cout << "This command requires access to LevelManager which is not available here." << std::endl;
    std::cout << "\nPress Enter to continue..." << std::endl;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    controller.notifyCommandExecuted("Save attempted via command (redirected to use 'V')");
}

std::string SaveGameCommand::toString() const {
    return "SaveGameCommand";
}

void QuitCommand::execute(GameController& controller) {
    std::cout << "\n=== QUIT TO MAIN MENU ===" << std::endl;
    std::cout << "To quit to main menu, use 'Q' in the main game loop." << std::endl;
    std::cout << "\nPress Enter to continue..." << std::endl;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    controller.notifyCommandExecuted("Quit attempted via command (redirected to use 'Q')");
}

std::string QuitCommand::toString() const {
    return "QuitCommand";
}

void HelpCommand::execute(GameController& controller) {
    InputHandler::showHelp();
}

std::string HelpCommand::toString() const {
    return "HelpCommand";
}

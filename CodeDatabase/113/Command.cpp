#include "Command.h"
#include "GameSession.h"
#include "Player.h"
#include <iostream>
#include <limits>

void MoveCommand::execute(GameSession& session) {
    auto& player = session.getPlayer();
    auto& field = session.getGameState().field;

    char direction;
    if (m_deltaX == -1) direction = 'a';
    else if (m_deltaX == 1) direction = 'd';
    else if (m_deltaY == -1) direction = 'w';
    else direction = 's';

    player.move(m_deltaX, m_deltaY, field);
    std::cout << "Moved ";
    if (m_deltaX == -1) std::cout << "left";
    else if (m_deltaX == 1) std::cout << "right";
    else if (m_deltaY == -1) std::cout << "up";
    else std::cout << "down";
    std::cout << std::endl;
}

void AttackCommand::execute(GameSession& session) {
    auto& player = session.getPlayer();
    auto& gameState = session.getGameState();

    if (player.areReferencesSet()) {
        player.attack(player.getEnemies(), player.getTowers(), gameState.field);
    }
}

void SwitchAttackCommand::execute(GameSession& session) {
    auto& player = session.getPlayer();
    player.switchAttackType();
}

void CastSpellCommand::execute(GameSession& session) {
    auto& player = session.getPlayer();
    auto& gameState = session.getGameState();

    // Проверяем наличие заклинаний
    if (player.getHand().getSpellCount() == 0) {
        std::cout << "No spells in hand!" << std::endl;
        return;
    }

    // Проверяем валидность индекса
    if (m_spellIndex < 0 || m_spellIndex >= static_cast<int>(player.getHand().getSpellCount())) {
        std::cout << "Invalid spell index!" << std::endl;
        return;
    }

    // Кастуем заклинание
    player.castSpell(m_spellIndex, gameState.field);
}

void SaveCommand::execute(GameSession& session) {
    if (session.saveGame()) {
        std::cout << "Game saved successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to save game!" << std::endl;
    }
}

void LoadCommand::execute(GameSession& session) {
    if (session.loadGame()) {
        std::cout << "Game loaded successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to load game!" << std::endl;
    }
}

void QuitCommand::execute(GameSession& session) {
    std::string confirm;
    std::cout << "Are you sure you want to quit? (y/n): ";
    std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        session.stop();
        std::cout << "Quitting game..." << std::endl;
    }
}

void InfoCommand::execute(GameSession& session) {
    auto& player = session.getPlayer();
    std::cout << "=== GAME INFO ===" << std::endl;
    std::cout << "Player position: (" << player.getPositionX()
        << ", " << player.getPositionY() << ")" << std::endl;
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << std::endl;
    std::cout << "Score: " << player.getScore() << std::endl;
}

std::unique_ptr<ICommand> CommandFactory::createCommand(char inputChar) {
    inputChar = static_cast<char>(std::tolower(static_cast<unsigned char>(inputChar)));

    switch (inputChar) {
    case 'w': return std::make_unique<MoveCommand>(0, -1);
    case 's': return std::make_unique<MoveCommand>(0, 1);
    case 'a': return std::make_unique<MoveCommand>(-1, 0);
    case 'd': return std::make_unique<MoveCommand>(1, 0);
    case 'e': return std::make_unique<AttackCommand>();
    case 'f': return std::make_unique<SwitchAttackCommand>();
    case 'c':
        return std::make_unique<CastSpellCommand>(0);
    case 'v': return std::make_unique<SaveCommand>();
    case 'l': return std::make_unique<LoadCommand>();
    case 'q': return std::make_unique<QuitCommand>();
    case 'i': return std::make_unique<InfoCommand>();
    default: return nullptr;
    }
}
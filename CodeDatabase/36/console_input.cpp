#include "console_input.h"
#include "concrete_commands.h"
#include <iostream>
#include <limits>

ConsoleInputHandler::ConsoleInputHandler() : key_config_() {
    // Конструктор key_config_ сам загрузит файл или настройки по умолчанию
}

std::unique_ptr<Command> ConsoleInputHandler::GetCommand(const World& context) {
    std::cout << "Action: ";
    char c;
    std::cin >> c;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::make_unique<NoOpCommand>();
    }

    // Получаем действие через конфиг
    GameAction action = key_config_.GetAction(c);

    switch (action) {
        // Движение теперь обрабатывается здесь, отдельные под-меню не нужны
        case GameAction::MoveUp:    return std::make_unique<MoveCommand>(-1, 0);
        case GameAction::MoveDown:  return std::make_unique<MoveCommand>(1, 0);
        case GameAction::MoveLeft:  return std::make_unique<MoveCommand>(0, -1);
        case GameAction::MoveRight: return std::make_unique<MoveCommand>(0, 1);
        
        case GameAction::SwitchMode: return std::make_unique<SwitchModeCommand>();
        
        case GameAction::Attack: return std::make_unique<AttackCommand>();
        
        case GameAction::CastSpell: {
            int idx = GetSpellIndex(context.GetPlayer());
            if (idx == -1) return std::make_unique<NoOpCommand>();
            auto target = GetTarget();
            return std::make_unique<CastSpellCommand>(idx, target);
        }
        
        case GameAction::SaveGame: {
            std::string fn = GetFilename(true);
            return std::make_unique<SaveCommand>(fn);
        }
        
        case GameAction::LoadGame: {
            std::string fn = GetFilename(false);
            return std::make_unique<LoadCommand>(fn);
        }
        
        case GameAction::Quit: return std::make_unique<QuitCommand>();
        
        default: 
            std::cout << "Unknown command or key not bound.\n";
            return std::make_unique<NoOpCommand>();
    }
}

std::pair<int, int> ConsoleInputHandler::GetTarget() {
    std::cout << "Enter target row and column: ";
    int r, c;
    while (!(std::cin >> r >> c)) {
        std::cout << "Invalid input. Enter two integers: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return {r, c};
}

int ConsoleInputHandler::GetSpellIndex(const Player& player) {
    int count = player.GetSpellCount();
    if (count == 0) {
        std::cout << "No spells available.\n";
        return -1;
    }
    std::cout << "Available Spells:\n";
    for (int i = 0; i < count; ++i) {
        const Spell* sp = player.GetSpell(i);
        if (sp) {
            std::cout << i << ": " << sp->Name() << " - " << sp->Description() << "\n";
        }
    }
    std::cout << "Choose spell index: ";
    int idx;
    std::cin >> idx;
    if (std::cin.fail() || idx < 0 || idx >= count) {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid spell choice.\n";
        return -1;
    }
    return idx;
}

int ConsoleInputHandler::GetUpgradeChoice() {
    std::cout << "\n--- LEVEL UP! Choose upgrade ---\n";
    std::cout << "1: Upgrade Health\n";
    std::cout << "2: Upgrade Damage\n";
    std::cout << "3: Upgrade Max Spells\n";
    std::cout << "4: Upgrade Enhancement Stacks\n";
    std::cout << "Enter choice (1-4): ";
    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 4) {
        std::cout << "Invalid choice. Try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return choice;
}

int ConsoleInputHandler::GetSaveLoadChoice() {
    std::cout << "1: Start New Game\n2: Load Game\nChoice: ";
    int c;
    std::cin >> c;
    return c;
}

std::string ConsoleInputHandler::GetFilename(bool save) {
    std::cout << "Enter filename to " << (save ? "save" : "load") << ": ";
    std::string s;
    std::cin >> s;
    return s;
}
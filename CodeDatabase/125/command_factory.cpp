#include "command_factory.h"
#include "move_command.h"
#include "attack_command.h"
#include "switch_mode_command.h"
#include "cast_spell_command.h"
#include "shop_command.h"
#include "save_command.h"
#include "input_config.h" 
#include <memory>
#include <iostream>

InputConfig CommandFactory::config_;

void CommandFactory::SetConfig(const InputConfig& config) {
    config_ = config;
}

std::unique_ptr<Command> CommandFactory::CreateCommandFromInput(char input) {
    if (input == '?') {
        std::cout << "\n======================== СПРАВКА ПО УПРАВЛЕНИЮ ========================\n";
        std::cout << InputConfig::GetInstance().GetHelpString();
        std::cout << "=======================================================================\n";
        return nullptr;
    }
    
    CommandType type = config_.GetCommandType(input);
    
    if (type == CommandType::Invalid) {
        std::cout << "Неизвестная команда!" << "\n";
        return nullptr;
    }
    
    return CreateCommandFromType(type);
}

std::unique_ptr<Command> CommandFactory::CreateCommandFromType(CommandType type, int param) {
    switch (type) {
        case CommandType::MoveUp:
            return std::make_unique<MoveCommand>(MoveCommand::Direction::Up);
        case CommandType::MoveDown:
            return std::make_unique<MoveCommand>(MoveCommand::Direction::Down);
        case CommandType::MoveLeft:
            return std::make_unique<MoveCommand>(MoveCommand::Direction::Left);
        case CommandType::MoveRight:
            return std::make_unique<MoveCommand>(MoveCommand::Direction::Right);
        case CommandType::Attack:
            return std::make_unique<AttackCommand>();
        case CommandType::SwitchMode:
            return std::make_unique<SwitchModeCommand>();
        case CommandType::CastSpell1:
            return std::make_unique<CastSpellCommand>(0);
        case CommandType::CastSpell2:
            return std::make_unique<CastSpellCommand>(1);
        case CommandType::CastSpell3:
            return std::make_unique<CastSpellCommand>(2);
        case CommandType::OpenShop:
            return std::make_unique<ShopCommand>();
        case CommandType::SaveGame:
            return std::make_unique<SaveCommand>();
        default:
            return nullptr;
    }
}

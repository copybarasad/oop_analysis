#include "concrete_commands.h"
#include "save_load_manager.h"
#include <iostream>

MoveCommand::MoveCommand(int dr, int dc) : dr_(dr), dc_(dc) {}
bool MoveCommand::Execute(World& world) {
    world.MovePlayer(dr_, dc_);
    return true; // Тратит ход
}

bool AttackCommand::Execute(World& world) {
    world.PlayerAttack();
    return true; // Тратит ход
}

bool SwitchModeCommand::Execute(World& world) {
    world.SwitchPlayerMode();
    return false; // Не тратит ход
}

CastSpellCommand::CastSpellCommand(int idx, std::pair<int, int> target) 
    : spell_idx_(idx), target_(target) {}

bool CastSpellCommand::Execute(World& world) {
    bool result = world.PlayerCastSpell(spell_idx_, target_);
    if (result) std::cout << "Spell cast successfully.\n";
    else std::cout << "Spell failed.\n";
    return result; // Тратит ход только при успехе
}

SaveCommand::SaveCommand(std::string filename) : filename_(std::move(filename)) {}
bool SaveCommand::Execute(World& world) {
    SaveLoadManager manager;
    try {
        manager.SaveGame(world, filename_);
        std::cout << "Game saved successfully.\n";
    } catch (const SaveLoadException& e) {
        std::cout << "Failed to save game: " << e.what() << "\n";
    }
    return false;
}

LoadCommand::LoadCommand(std::string filename) : filename_(std::move(filename)) {}
bool LoadCommand::Execute(World& world) {
    SaveLoadManager manager;
    try {
        // Загружаем новый мир и присваиваем текущему (используется move assignment)
        world = manager.LoadGame(filename_);
        std::cout << "Game loaded successfully.\n";
    } catch (const SaveLoadException& e) {
        std::cout << "Failed to load game: " << e.what() << "\n";
    }
    return false;
}

bool QuitCommand::Execute(World& world) {
    return false;
}
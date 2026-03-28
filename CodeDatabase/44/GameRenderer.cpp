#include "GameRenderer.h"
#include "Field.h"
#include "Player.h"
#include "Cell.h"
#include <iostream>

GameRenderer::GameRenderer() : key_bindings_() {
    // KeyBindings инициализируется с дефолтными значениями
}

void GameRenderer::SetKeyBindings(const KeyBindings& bindings) {
    key_bindings_ = bindings;
}

void GameRenderer::RenderField(const Field& field) const {
    int width = field.GetWidth();
    int height = field.GetHeight();

    // Заголовок с номерами столбцов
    std::cout << "    ";
    for (int j = 0; j < width; ++j) {
        if (j < 10) {
            std::cout << " " << j << " ";
        } else {
            std::cout << j << " ";
        }
    }
    std::cout << "\n";

    // Верхняя граница
    std::cout << "  +";
    for (int j = 0; j < width; ++j) {
        std::cout << "---";
    }
    std::cout << "--+\n";

    // Строки поля
    for (int i = 0; i < height; ++i) {
        if (i < 10) {
            std::cout << " " << i << "|  ";
        } else {
            std::cout << i << "|  ";
        }

        for (int j = 0; j < width; ++j) {
            CellType type = field.GetCellType(j, i);
            const char* color = GetCellColor(static_cast<int>(type));
            char symbol = GetCellSymbol(static_cast<int>(type));
            std::cout << color << symbol << kReset << "  ";
        }
        std::cout << "|\n";
    }

    // Нижняя граница
    std::cout << "  +";
    for (int j = 0; j < width; ++j) {
        std::cout << "---";
    }
    std::cout << "--+\n";
}

void GameRenderer::RenderPlayerStatus(const Player& player, int spell_count) const {
    std::cout << player.GetDescription()
              << " | Количество заклинаний: " << spell_count << "\n";
}

void GameRenderer::RenderCommands() const {
    char up = key_bindings_.GetKeyForCommand(CommandType::MOVE_UP);
    char down = key_bindings_.GetKeyForCommand(CommandType::MOVE_DOWN);
    char left = key_bindings_.GetKeyForCommand(CommandType::MOVE_LEFT);
    char right = key_bindings_.GetKeyForCommand(CommandType::MOVE_RIGHT);
    char attack = key_bindings_.GetKeyForCommand(CommandType::ATTACK);
    char switch_weapon = key_bindings_.GetKeyForCommand(CommandType::SWITCH_WEAPON);
    char use_spell = key_bindings_.GetKeyForCommand(CommandType::USE_SPELL);
    char view_spells = key_bindings_.GetKeyForCommand(CommandType::VIEW_SPELLS);
    char save_game = key_bindings_.GetKeyForCommand(CommandType::SAVE_GAME);
    char load_game = key_bindings_.GetKeyForCommand(CommandType::LOAD_GAME);
    char quit = key_bindings_.GetKeyForCommand(CommandType::QUIT);

    std::cout << "Команды:\n"
              << "  " << up << "/" << left << "/" << down << "/" << right << " - движение\n"
              << "  " << attack << " - атака\n"
              << "  " << switch_weapon << " - смена оружия\n"
              << "  " << use_spell << " - использовать заклинание\n"
              << "  " << view_spells << " - посмотреть доступные заклинания\n"
              << "  " << save_game << " - сохранить игру\n"
              << "  " << load_game << " - загрузить игру\n"
              << "  " << quit << " - выйти из игры\n";
}

void GameRenderer::RenderGameState(const Field& field, const Player& player, int spell_count) const {
    RenderField(field);
    RenderPlayerStatus(player, spell_count);
    RenderCommands();
    ShowCommandPrompt();
}

void GameRenderer::RenderMainMenu() const {
    std::cout << "=== Добро пожаловать в игру! ===\n";
    std::cout << "1. Начать новую игру\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Выйти\n";
    std::cout << "Выберите действие: ";
}

void GameRenderer::RenderLevelUpMenu() const {
    std::cout << "\n=== Прокачка! ===\n";
    std::cout << "1. +20 к максимальному здоровью\n";
    std::cout << "2. +2 к урону\n";
    std::cout << "3. Получить случайное заклинание\n";
    std::cout << "Выберите улучшение для следующего уровня: ";
}

void GameRenderer::RenderSavesList(const std::vector<std::string>& saves) const {
    std::cout << "=== Список сохранений ===\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i << ") " << saves[i] << "\n";
    }
}

void GameRenderer::ShowMessage(const std::string& message) const {
    std::cout << message << "\n";
}

void GameRenderer::ShowError(const std::string& error) const {
    std::cout << kRed << error << kReset << "\n";
}

void GameRenderer::ShowVictory() const {
    std::cout << kGreen << "ПОБЕДА! Все враги побеждены." << kReset << "\n";
}

void GameRenderer::ShowDefeat() const {
    std::cout << kRed << "Вы погибли!" << kReset << "\n";
}

void GameRenderer::ShowLevelTransition(int level) const {
    std::cout << "\n=== Переход на уровень " << level << " ===\n";
    std::cout << "Ваше здоровье восстановлено.\n";
    std::cout << "Половина заклинаний утеряна.\n";
    std::cout << "Враги стали сильнее!\n\n";
}

void GameRenderer::ShowGameComplete() const {
    std::cout << "\n=== Поздравляем! ===\n";
    std::cout << "Вы прошли всю игру!\n\n";
}

void GameRenderer::ShowPrompt(const std::string& prompt) const {
    std::cout << prompt;
}

void GameRenderer::ShowCommandPrompt() const {
    std::cout << "Введите команду: ";
}

char GameRenderer::GetCellSymbol(int cell_type) const {
    switch (static_cast<CellType>(cell_type)) {
        case CellType::EMPTY:    return '.';
        case CellType::ENEMY:    return 'E';
        case CellType::PLAYER:   return 'P';
        case CellType::WALL:     return '#';
        case CellType::SLOW:     return '~';
        case CellType::BUILDING: return 'B';
        case CellType::TRAP:     return '^';
        case CellType::TOWER:    return 'T';
        default:                 return '?';
    }
}

const char* GameRenderer::GetCellColor(int cell_type) const {
    switch (static_cast<CellType>(cell_type)) {
        case CellType::EMPTY:    return kGray;
        case CellType::ENEMY:    return kRed;
        case CellType::PLAYER:   return kGreen;
        case CellType::WALL:     return kGray;
        case CellType::SLOW:     return kCyan;
        case CellType::BUILDING: return kMagenta;
        case CellType::TRAP:     return kYellow;
        case CellType::TOWER:    return kMagenta;
        default:                 return kReset;
    }
}

#include "game_renderer.h"
#include "command_registry.h"
#include "key_bindings_config.h"
#include <iostream>
#include <iomanip>

void GameRenderer::render(const Game& game) {
    renderGameState(game.getContext());
}

void GameRenderer::renderGameState(const ConstGameContext& context) {
    displayTurnInfo(context);
    displayField(context);  
}

void GameRenderer::displayField(const ConstGameContext& context) {
    std::cout << "\n🗺️  КАРТА ЗАМКА\n";
    std::cout << "──────────────\n";
    
    int width = context.field.getWidth();
    int height = context.field.getHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            std::cout << getCellSymbol(pos, context) << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n📝 ЛЕГЕНДА: H-Охотник D-Дракула T-Башня L-Библиотека C-Часовня A-Оружейная V-Логово G-Гробница #-Стены\n";
}

char GameRenderer::getCellSymbol(const Position& pos, const ConstGameContext& context) const {
    CellType cell = context.field.getCellType(pos);
    
    if (pos == context.hunter.getPosition()) {
        return 'H';
    }
    else if (pos == context.dracula.getPosition()) {
        return 'D';
    }
    else if (context.field.hasTowerAt(pos)) {
        return 'T';
    }
    else if (context.field.getLibraryAt(pos) != nullptr) {
        return 'L';
    }
    else if (cell == CellType::WALL) {
        return '#';
    }
    else if (cell == CellType::CHAPEL) {
        return 'C';
    }
    else if (cell == CellType::ARMORY) {
        return 'A';
    }
    else if (cell == CellType::VAMPIRE_LAIR) {
        return 'V';
    }
    else if (cell == CellType::DRACULA_TOMB) {
        return 'G';
    }
    
    return '.';
}

void GameRenderer::displayStatus(const ConstGameContext& context) {
    std::cout << "\n📊 СТАТУС ПЕРСОНАЖЕЙ\n";
    std::cout << "────────────────────\n";
    
    std::cout << "🕵️ " << context.hunter.getName() << "\n";
    std::cout << "   Здоровье: ";
    printHealthBar(context.hunter.getHealth(), context.hunter.getMaxHealth());
    std::cout << "   " << context.hunter.getHealth() << "/" << context.hunter.getMaxHealth() << " HP\n";
    
    std::cout << "   Исследования: " << context.hunter.getResearchProgress() << "/3 ";
    for (int i = 0; i < 3; ++i) {
        std::cout << (i < context.hunter.getResearchProgress() ? "🔬" : "○");
    }
    std::cout << "\n";
    
    std::cout << "   Режим: " << (context.hunter.isRangedMode() ? "🏹 ДАЛЬНИЙ БОЙ" : "🗡️ БЛИЖНИЙ БОЙ") << "\n";
    std::cout << "   Урон: " << context.hunter.getCurrentAttackDamage() << "\n";
    std::cout << "   Позиция: (" << context.hunter.getPosition().getX() << ", " << context.hunter.getPosition().getY() << ")\n";
    std::cout << "   Пули: " << context.hunter.getSilverBullets() << " 🏹 |  Колья: " << context.hunter.getOakStakes() << " 🗡️\n";
    
    std::cout << "\n🧛 " << context.dracula.getName() << "\n";
    std::cout << "   Здоровье: ";
    printHealthBar(context.dracula.getHealth(), 225);  
    std::cout << "   " << context.dracula.getHealth() << " HP\n";
    
    std::cout << "   Состояние: " << (context.dracula.getFearLevel() == 0 ? "😠 АГРЕССИЯ" : "😨 ПАНИКА") << "\n";
    std::cout << "   Позиция: (" << context.dracula.getPosition().getX() << ", " << context.dracula.getPosition().getY() << ")\n";
    
    if (context.dracula.isInvulnerable()) {
        std::cout << "   ⚠️  НЕУЯЗВИМ (найдите гробницу)\n";
    }
    std::cout << "\n";
}

void GameRenderer::printHealthBar(int current, int max, int width) const {
    int filled = (current * width) / max;
    if (filled > width) filled = width;
    
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] ";
}

void GameRenderer::displaySpells(const VampireHunter& hunter) {
    std::cout << "\n📜 ЗАКЛИНАНИЯ В РУКЕ\n";
    std::cout << "──────────────────\n";
    
    const SpellHand& hand = hunter.getSpellHand();
    int spellCount = hand.getSpellCount();
    
    if (spellCount == 0) {
        std::cout << "У вас нет заклинаний\n";
        return;
    }
    
    for (int i = 0; i < spellCount; ++i) {
        Spell* spell = hand.getSpell(i);
        if (spell) {
            std::cout << i + 1 << ". " << spell->getName() 
                     << " (дистанция: " << spell->getRange() << " клеток)\n";
        }
    }
    std::cout << "\n";
}

void GameRenderer::displayHelp(const CommandRegistry& commandRegistry) {
    auto commandsByCategory = commandRegistry.getCommandsByCategory();
    displayHelp(commandsByCategory);
}

void GameRenderer::displayHelp(const KeyBindingsConfig& keyBindings) {
    auto commandsByCategory = keyBindings.getBindingsByCategory();
    displayHelp(commandsByCategory);
}

void GameRenderer::displayHelp(const std::map<std::string, 
                              std::vector<std::pair<std::string, std::string>>>& commandsByCategory) {
    std::cout << "\n📋 ДОСТУПНЫЕ КОМАНДЫ\n";
    std::cout << "────────────────────\n";
    
    for (const auto& category : commandsByCategory) {
        std::cout << category.first << ":\n";
        for (const auto& command : category.second) {
            std::cout << "  " << std::left << std::setw(12) << command.first 
                     << " - " << command.second << "\n";
        }
        std::cout << "\n";
    }
}

void GameRenderer::displayMessage(const std::string& message) {
    std::cout << message << "\n";
}

void GameRenderer::displayTurnInfo(const ConstGameContext& context) {
    std::cout << "\n";
    std::cout << "❤️ Охотник: " << context.hunter.getHealth() << "/" << context.hunter.getMaxHealth() << " HP";
    std::cout << " | 🧛 Дракула: " << context.dracula.getHealth() << " HP";
    
    if (context.dracula.isInvulnerable()) {
        std::cout << " | 💫 БЕССМЕРТЕН";
    }
    
    std::cout << " | 🔬 Исследования: " << context.hunter.getResearchProgress() << "/3";
    
    if (context.hunter.isReadyForFinalBlow()) {
        std::cout << " 🎯";
    }
    std::cout << "\n";
    
    std::cout << "Режим: " << (context.hunter.isRangedMode() ? "🏹 ДАЛЬНИЙ" : "🗡️ БЛИЖНИЙ");
    std::cout << " | Пули: " << context.hunter.getSilverBullets();
    std::cout << " | Колья: " << context.hunter.getOakStakes();
    std::cout << " | Заклинания: " << context.hunter.getSpellHand().getSpellCount() 
              << "/" << context.hunter.getSpellHand().getMaxSlots();
    std::cout << "\n";
}

void GameRenderer::displayLevelInfo(const ConstGameContext& context, int currentLevel, 
                                   const LevelConfig& config) {
    std::cout << "\n🦇══════════════════════════════════════🦇\n";
    std::cout << "               УРОВЕНЬ " << currentLevel << "\n";
    std::cout << "🦇══════════════════════════════════════🦇\n";
    
    if (currentLevel == 1) {
        std::cout << "Вы проникли в первый зал замка Дракулы.\n";
        std::cout << "Здесь он слабее всего - ваша лучшая возможность!\n";
    }
    else if (currentLevel == 6) {
        std::cout << "💀 ФИНАЛЬНАЯ СХВАТКА В СЕРДЦЕ ЗАМКА! 💀\n";
        std::cout << "Дракула обрёл БЕССМЕРТИЕ в своей гробнице!\n";
        std::cout << "Найдите гробницу и лишите его силы, иначе все усилия напрасны!\n";
        std::cout << "⚰️  Гробница отмечена символом 'G' на карте\n";
    }
    else {
        std::cout << "Дракула переродился и стал СИЛЬНЕЕ!\n";
        std::cout << "С каждым уровнем его мощь растёт...\n";
    }
    
    // Информация о башнях
    if (config.towersCount > 0) {
        std::cout << "\n🏰 На уровне: " << config.towersCount << " вампирских башен\n";
        std::cout << "   Башни атакуют на расстоянии 3 клеток каждые 6 ходов\n";
        std::cout << "   Обозначение на карте: 'T'\n";
    }
    
    std::cout << "\n";
}

void GameRenderer::displayWelcome() {
    std::cout << "\n";
    std::cout << "🦇══════════════════════════════════════🦇\n";
    std::cout << "         ОХОТНИК НА ВАМПИРОВ\n"; 
    std::cout << "🦇══════════════════════════════════════🦇\n";
    std::cout << "\n";
    std::cout << "Вы проникли в мрачный замок Дракулы в Трансильвании.\n";
    std::cout << "Стены хранят древние тайны, а в темных коридорах скрывается сам вампир.\n";
    std::cout << "\n";
    std::cout << "🎯 СИСТЕМА РЕСУРСОВ:\n";
    std::cout << "- Серебряные пули: для дальнего боя (ограниченный запас)\n";
    std::cout << "- Осиновые колья: для ближнего боя (ограниченный запас)\n"; 
    std::cout << "- Исследования: увеличивают урон и раскрывают слабости Дракулы\n";
    std::cout << "- Заклинания: используйте [F] для применения\n";
    std::cout << "\n";
    std::cout << "⚡ Используйте тактику и ресурсы мудро чтобы победить!\n";
    std::cout << "\n";
}

void GameRenderer::displayQuickHelp() {
    std::cout << "\n💡 КОМАНДЫ: [WASD]движение [ПРОБЕЛ]атака [B]бой [F]заклинание [P]список [I]статус [O]сохранить [L]загрузить [H]помощь [Q]выход\n";
}

void GameRenderer::displayLibraryChoice(const Library& library) {
    std::cout << "\n📚 ВЫ НАШЛИ БИБЛИОТЕКУ!\n";
    
    std::string spellName;
    switch (library.getAvailableSpell()) {
        case SpellType::HOLY_WATER: spellName = "💧 Святая Вода (ловушка)"; break;
        case SpellType::LIGHT_BEAM: spellName = "✨ Луч Света (прямой урон)"; break;
        case SpellType::SACRED_GLOW: spellName = "☀️ Священное Сияние (площадь)"; break;
    }
    
    std::cout << "Доступное заклинание: " << spellName << "\n\n";
    std::cout << "Выберите действие:\n";
    std::cout << "1. Изучить заклинание '" << spellName << "'\n";
    std::cout << "2. Провести исследование Дракулы (+1 к исследованию)\n";
    std::cout << "Ваш выбор (1 или 2): ";
}

void GameRenderer::displayLevelUpOptions(const std::vector<PlayerUpgradeSystem::UpgradeOption>& upgrades) {
    std::cout << "\n🎁 ВЫБЕРИТЕ УЛУЧШЕНИЕ ДЛЯ ОХОТНИКА:\n";
    std::cout << "────────────────────────────\n";
    
    for (const auto& upgrade : upgrades) {
        std::cout << upgrade.id << ". " << upgrade.description << "\n";
    }
    
    std::cout << "\nВаш выбор (1-" << upgrades.size() << "): ";
}

std::string GameRenderer::getPlayerChoice() {
    std::string choice;
    std::getline(std::cin, choice);
    return choice;
}

Position GameRenderer::getSpellTarget() {
    std::cout << "🎯 Введите координаты цели (X Y): ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        size_t spacePos = input.find(' ');
        if (spacePos == std::string::npos) {
            throw std::invalid_argument("Неверный формат");
        }
        
        int x = std::stoi(input.substr(0, spacePos));
        int y = std::stoi(input.substr(spacePos + 1));
        
        return Position(x, y);
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Неверные координаты. Используйте формат: X Y");
    }
}

int GameRenderer::getSpellIndex(int maxIndex) {
    std::cout << "Выберите заклинание (1-" << maxIndex << "): ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        int index = std::stoi(input) - 1;
        if (index < 0 || index >= maxIndex) {
            throw std::out_of_range("Неверный номер");
        }
        return index;
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Неверный выбор. Введите число от 1 до " + std::to_string(maxIndex));
    }
}

int GameRenderer::getUpgradeChoice(int maxChoice) {
    std::cout << "Ваш выбор (1-" << maxChoice << "): ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        int choice = std::stoi(input);
        if (choice < 1 || choice > maxChoice) {
            throw std::out_of_range("Неверный номер");
        }
        return choice;
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Неверный выбор. Введите число от 1 до " + std::to_string(maxChoice));
    }
}
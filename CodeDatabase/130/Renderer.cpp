#include "Renderer.h"
#include "Game.h"
#include <iostream>
#include <sstream>

void ConsoleRenderer::render(Game& game) {
    GameRenderData data = game.getRenderData();
    renderFromData(data);
}

void ConsoleRenderer::renderFromData(const GameRenderData& data) {
    std::cout << "\n=== Turn " << data.currentTurn << " ===\n";
    renderPlayerInfo(data);
    renderField(data);
    renderMenu(data);
}

void ConsoleRenderer::renderField(const GameRenderData& data) {
    std::cout << "+";
    for (int x = 0; x < data.fieldWidth; ++x) std::cout << "--";
    std::cout << "+\n";

    auto hasPos = [](const std::vector<Pos>& positions, int x, int y) {
        for (const auto& p : positions) {
            if (p.x == x && p.y == y) return true;
        }
        return false;
    };

    for (int y = 0; y < data.fieldHeight; ++y) {
        std::cout << "|";
        for (int x = 0; x < data.fieldWidth; ++x) {
            if (data.playerPos.x == x && data.playerPos.y == y) {
                std::cout << "P ";
            } else if (hasPos(data.enemyPositions, x, y)) {
                std::cout << "E ";
            } else if (hasPos(data.allyPositions, x, y)) {
                std::cout << "A ";
            } else if (hasPos(data.buildingPositions, x, y)) {
                std::cout << "B ";
            } else if (hasPos(data.towerPositions, x, y)) {
                std::cout << "T ";
            } else if (hasPos(data.trapPositions, x, y)) {
                std::cout << "X ";
            } else {
                CellType ct = data.cells[y][x];
                if (ct == CellType::BLOCKED) std::cout << "# ";
                else if (ct == CellType::SLOW) std::cout << "~ ";
                else std::cout << ". ";
            }
        }
        std::cout << "|\n";
    }

    std::cout << "+";
    for (int x = 0; x < data.fieldWidth; ++x) std::cout << "--";
    std::cout << "+\n";
    std::cout << "Легенда: P=Игрок, E=Враг, A=Союзник, B=Здание, T=Башня, X=Ловушка, #=Непроходимая, ~=Замедленная, .=Пустая\n";
}

void ConsoleRenderer::renderPlayerInfo(const GameRenderData& data) {
    std::cout << "Player: HP=" << data.playerHealth
              << "/" << data.playerMaxHealth
              << " Dmg=" << data.playerDamage
              << " Score=" << data.playerScore
              << " Killed=" << data.enemiesKilled
              << " Mode=" << (data.playerMode == CombatMode::MELEE ? "Melee" : "Ranged");
    if (!data.playerCanMove) std::cout << " [Can't move]";
    if (data.playerSlowed) std::cout << " [Slowed]";
    std::cout << "\n";
    std::cout << "Enemies: " << data.enemyCount
              << " | Buildings: " << data.buildingCount
              << " | Allies: " << data.allyCount
              << " | Towers: " << data.towerCount
              << " | Traps: " << data.trapCount << "\n";
    renderHand(data);
}

void ConsoleRenderer::renderHand(const GameRenderData& data) {
    if (data.handSpells.empty()) {
        std::cout << "Hand: (empty)\n";
        return;
    }
    std::cout << "Hand [" << data.handSpells.size() << "/" << data.handMaxSize << "]: ";
    for (size_t i = 0; i < data.handSpells.size(); ++i) {
        const auto& spell = data.handSpells[i];
        std::cout << i << " - " << spell.name;
        bool hasEnhancements = (data.damageBonus > 0 || data.rangeBonus > 0 || 
                                data.areaBonus > 0 || data.summonBonus > 0);
        if (hasEnhancements && spell.type != SpellType::ENHANCE) {
            std::string buff = formatEnhancement(data);
            if (!buff.empty()) std::cout << " [" << buff << "]";
        }
        if (i < data.handSpells.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";
}

void ConsoleRenderer::renderMenu(const GameRenderData& data) {
    std::cout << "Commands: "
              << getKeyForCommand(data, CommandType::MOVE_UP, 'W') << "/"
              << getKeyForCommand(data, CommandType::MOVE_LEFT, 'A') << "/"
              << getKeyForCommand(data, CommandType::MOVE_DOWN, 'S') << "/"
              << getKeyForCommand(data, CommandType::MOVE_RIGHT, 'D') << " - Move, "
              << getKeyForCommand(data, CommandType::ATTACK, 'F') << " - Attack, "
              << getKeyForCommand(data, CommandType::CAST_SPELL, 'C') << " - Cast Spell, "
              << getKeyForCommand(data, CommandType::SAVE, 'V') << " - Save, "
              << getKeyForCommand(data, CommandType::SWITCH_MODE, 'T') << " - Switch Mode, "
              << getKeyForCommand(data, CommandType::QUIT, 'Q') << " - Quit\n";
}

std::string ConsoleRenderer::formatEnhancement(const GameRenderData& data) const {
    std::vector<std::string> parts;
    if (data.damageBonus > 0) parts.push_back("+" + std::to_string(data.damageBonus) + " dmg");
    if (data.rangeBonus > 0) parts.push_back("+" + std::to_string(data.rangeBonus) + " rng");
    if (data.areaBonus > 0) parts.push_back("+" + std::to_string(data.areaBonus) + " area");
    if (data.summonBonus > 0) parts.push_back("+" + std::to_string(data.summonBonus) + " summons");
    if (parts.empty()) return "";
    std::ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << parts[i];
    }
    return oss.str();
}

char ConsoleRenderer::getKeyForCommand(const GameRenderData& data, CommandType type, char fallback) const {
    auto it = data.keyBindings.find(type);
    return it != data.keyBindings.end() ? it->second : fallback;
}

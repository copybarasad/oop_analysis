#ifndef OOP_CONSOLE_RENDERER_H_
#define OOP_CONSOLE_RENDERER_H_

#include "game.h"
#include <iostream>
#include <string>
#include <optional>
#include <vector>

namespace rpg {
    struct Upgrade;

    class ConsoleRenderer final {
    public:
        ConsoleRenderer() = default;

        void RenderGameState(const Game &game) const;

        void RenderMap(const Game &game) const;

        void RenderHeader() const;

        void ClearScreen() const;

        void RenderMessage(const std::string &message) const;

        void RenderMainMenu() const;

        void RenderUpgradeMenu(const std::vector<Upgrade> &upgrades) const;

        void RenderLevelComplete(int score) const;

        void RenderGameComplete(int final_score) const;

        void RenderLevelStart(int level_number) const;

        void RenderGameOver(int score, int turns, int level) const;

        [[nodiscard]] std::optional<Position> SelectTarget(
            const Game &game,
            const Position &player_pos,
            int range,
            int area_size = 1,
            SpellType spell_type = SpellType::kDirectDamage) const;

    private:
        [[nodiscard]] char GetCharFor(const Player &player) const;

        [[nodiscard]] char GetCharFor(const Enemy &enemy) const;

        [[nodiscard]] char GetCharFor(const EnemyBuilding &building) const;

        [[nodiscard]] char GetCharFor(const Trap &trap) const;

        [[nodiscard]] char GetCharFor(const Ally &ally) const;

        [[nodiscard]] char GetCharFor(const EnemyTower &tower) const;

        [[nodiscard]] char GetCharFor(CellType cell_type) const;

        void RenderTargetSelectionMap(
            const Game &game,
            const Position &cursor_pos,
            const Position &player_pos,
            int range,
            int area_size,
            SpellType spell_type) const;

        [[nodiscard]] bool IsInRange(
            const Position &from,
            const Position &to,
            int range) const;

        [[nodiscard]] std::vector<Position> GetAffectedPositions(
            const Position &center,
            int area_size,
            const GameField &field) const;

        [[nodiscard]] int GetInput() const;
    };
}

#endif

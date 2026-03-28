#ifndef OOP_GAME_VISUALIZER_H_
#define OOP_GAME_VISUALIZER_H_

#include "game.h"
#include "position.h"
#include <string>
#include <optional>

namespace rpg {
    template<typename TRenderer>
    class GameVisualizer final {
    public:
        GameVisualizer() : renderer_() {}

        void Visualize(const Game& game) const {
            renderer_.ClearScreen();
            renderer_.RenderHeader();
            renderer_.RenderGameState(game);
            renderer_.RenderMap(game);
        }

        void VisualizeWithMessage(const Game& game, const std::string& message) const {
            renderer_.ClearScreen();
            renderer_.RenderHeader();
            
            if (!message.empty()) {
                renderer_.RenderMessage(message);
            }
            
            renderer_.RenderGameState(game);
            renderer_.RenderMap(game);
        }

        void ShowHeader() const {
            renderer_.ClearScreen();
            renderer_.RenderHeader();
        }

        void Clear() const {
            renderer_.ClearScreen();
        }

        [[nodiscard]] const TRenderer& GetRenderer() const {
            return renderer_;
        }

        [[nodiscard]] std::optional<Position> SelectTarget(
            const Game& game,
            const Position& player_pos,
            int range,
            int area_size = 1,
            SpellType spell_type = SpellType::kDirectDamage) const {
            return renderer_.SelectTarget(game, player_pos, range, area_size, spell_type);
        }

    private:
        TRenderer renderer_;
    };
}

#endif


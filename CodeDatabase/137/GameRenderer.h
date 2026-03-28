#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <string>

class IGameRenderer {
public:
    virtual ~IGameRenderer() = default;
    virtual void Render(const Game& game) = 0;
    virtual void HandleClick(int mouse_x, int mouse_y, Game& game) = 0;
};

class SfmlGameRenderer : public IGameRenderer {
public:
    explicit SfmlGameRenderer(sf::RenderWindow& window);

    void Render(const Game& game) override;
    void HandleClick(int mouse_x, int mouse_y, Game& game) override;

    int GetSelectedSpellIndex() const;
    void SetSelectedSpellIndex(int index);
    bool IsSelectingTarget() const;
    void SetSelectingTarget(bool selecting);
    sf::Vector2i GetMouseCellPosition(int mouse_x, int mouse_y, const Game& game) const;

private:
    void DrawField(const Game& game);
    void DrawEntities(const Game& game);
    void DrawUI(const Game& game);
    void DrawMenu(const Game& game);
    void DrawPauseMenu(const Game& game);
    void DrawLevelComplete(const Game& game);
    void DrawUpgradeMenu(const Game& game);
    void DrawGameOver(const Game& game);
    void DrawHealthBar(int x, int y, int width, int height, int current, int max, sf::Color bar_color);
    sf::Color GetCellColor(const Cell& cell) const;

    sf::RenderWindow& window_;
    sf::Font font_;
    int cell_size_;
    int offset_x_;
    int offset_y_;

    int selected_spell_index_;
    bool selecting_target_;
    int target_x_;
    int target_y_;
};

#endif // GAME_RENDERER_H

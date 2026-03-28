#include "GameRenderer.h"
#include <sstream>

SfmlGameRenderer::SfmlGameRenderer(sf::RenderWindow& window)
    : window_(window),
      cell_size_(40),
      offset_x_(50),
      offset_y_(50),
      selected_spell_index_(-1),
      selecting_target_(false),
      target_x_(-1),
      target_y_(-1) {
    std::vector<std::string> font_paths = {
        "/Library/Fonts/Lato-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/Library/Fonts/Arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "arial.ttf"
    };

    for (const auto& path : font_paths) {
        if (font_.openFromFile(path)) {
            break;
        }
    }
}

sf::Color SfmlGameRenderer::GetCellColor(const Cell& cell) const {
    if (cell.GetType() == CellType::WALL) {
        return sf::Color(140, 140, 160);
    }
    if (cell.GetType() == CellType::SLOWING) {
        return sf::Color(200, 220, 255);
    }
    if (cell.HasTrap()) {
        return sf::Color(255, 200, 200);
    }
    return sf::Color(245, 245, 240);
}

void SfmlGameRenderer::DrawField(const Game& game) {
    const GameField* field = game.GetField();
    if (!field) return;

    for (int y = 0; y < field->GetHeight(); ++y) {
        for (int x = 0; x < field->GetWidth(); ++x) {
            const Cell& cell = field->GetCell(x, y);

            sf::RectangleShape cell_shape(sf::Vector2f(cell_size_ - 2, cell_size_ - 2));
            cell_shape.setPosition(sf::Vector2f(offset_x_ + x * cell_size_, offset_y_ + y * cell_size_));
            cell_shape.setFillColor(GetCellColor(cell));
            cell_shape.setOutlineColor(sf::Color(200, 200, 200));
            cell_shape.setOutlineThickness(1);

            window_.draw(cell_shape);
        }
    }
}

void SfmlGameRenderer::DrawEntities(const Game& game) {
    const GameField* field = game.GetField();
    if (!field) return;

    for (const auto& building : game.GetEnemyBuildings()) {
        sf::RectangleShape shape(sf::Vector2f(cell_size_ - 8, cell_size_ - 8));
        shape.setPosition(sf::Vector2f(offset_x_ + building->GetX() * cell_size_ + 4,
                                       offset_y_ + building->GetY() * cell_size_ + 4));
        shape.setFillColor(sf::Color(180, 100, 100));
        window_.draw(shape);

        DrawHealthBar(offset_x_ + building->GetX() * cell_size_,
                      offset_y_ + building->GetY() * cell_size_ - 8,
                      cell_size_, 6,
                      building->GetHealth(), building->GetMaxHealth(),
                      sf::Color(200, 100, 100));
    }

    for (const auto& tower : game.GetEnemyTowers()) {
        sf::CircleShape shape(cell_size_ / 2 - 6);
        shape.setPosition(sf::Vector2f(offset_x_ + tower->GetX() * cell_size_ + 6,
                                       offset_y_ + tower->GetY() * cell_size_ + 6));
        shape.setFillColor(sf::Color(120, 100, 150));
        window_.draw(shape);

        DrawHealthBar(offset_x_ + tower->GetX() * cell_size_,
                      offset_y_ + tower->GetY() * cell_size_ - 8,
                      cell_size_, 6,
                      tower->GetHealth(), tower->GetMaxHealth(),
                      sf::Color(150, 100, 180));
    }

    for (const auto& enemy : game.GetEnemies()) {
        sf::CircleShape shape(cell_size_ / 2 - 4);
        shape.setPosition(sf::Vector2f(offset_x_ + enemy->GetX() * cell_size_ + 4,
                                       offset_y_ + enemy->GetY() * cell_size_ + 4));
        shape.setFillColor(sf::Color(255, 150, 150));
        window_.draw(shape);

        DrawHealthBar(offset_x_ + enemy->GetX() * cell_size_,
                      offset_y_ + enemy->GetY() * cell_size_ - 8,
                      cell_size_, 6,
                      enemy->GetHealth(), enemy->GetMaxHealth(),
                      sf::Color(255, 100, 100));
    }

    for (const auto& ally : game.GetAllies()) {
        sf::CircleShape shape(cell_size_ / 2 - 4);
        shape.setPosition(sf::Vector2f(offset_x_ + ally->GetX() * cell_size_ + 4,
                                       offset_y_ + ally->GetY() * cell_size_ + 4));
        shape.setFillColor(sf::Color(150, 255, 150));
        window_.draw(shape);

        DrawHealthBar(offset_x_ + ally->GetX() * cell_size_,
                      offset_y_ + ally->GetY() * cell_size_ - 8,
                      cell_size_, 6,
                      ally->GetHealth(), ally->GetMaxHealth(),
                      sf::Color(100, 255, 100));
    }

    const Player* player = game.GetPlayer();
    if (player) {
        sf::CircleShape shape(cell_size_ / 2 - 4);
        shape.setPosition(sf::Vector2f(offset_x_ + player->GetX() * cell_size_ + 4,
                                       offset_y_ + player->GetY() * cell_size_ + 4));
        shape.setFillColor(sf::Color(150, 200, 255));
        window_.draw(shape);

        DrawHealthBar(offset_x_ + player->GetX() * cell_size_,
                      offset_y_ + player->GetY() * cell_size_ - 8,
                      cell_size_, 6,
                      player->GetHealth(), player->GetMaxHealth(),
                      sf::Color(100, 150, 255));
    }
}

void SfmlGameRenderer::DrawHealthBar(int x, int y, int width, int height, int current, int max, sf::Color bar_color) {
    sf::RectangleShape bg_bar(sf::Vector2f(width, height));
    bg_bar.setPosition(sf::Vector2f(x, y));
    bg_bar.setFillColor(sf::Color(50, 50, 50));
    window_.draw(bg_bar);

    if (max > 0) {
        float ratio = static_cast<float>(current) / max;
        sf::RectangleShape health_bar(sf::Vector2f(width * ratio, height));
        health_bar.setPosition(sf::Vector2f(x, y));
        health_bar.setFillColor(bar_color);
        window_.draw(health_bar);
    }
}

void SfmlGameRenderer::DrawUI(const Game& game) {
    const GameField* field = game.GetField();
    const Player* player = game.GetPlayer();
    const PlayerHand* hand = game.GetPlayerHand();
    if (!player || !hand) return;

    int ui_x = offset_x_ + (field ? field->GetWidth() * cell_size_ + 30 : 30);
    int ui_y = offset_y_;

    sf::RectangleShape ui_panel(sf::Vector2f(300, 700));
    ui_panel.setPosition(sf::Vector2f(ui_x - 10, ui_y - 10));
    ui_panel.setFillColor(sf::Color(250, 250, 245, 200));
    ui_panel.setOutlineColor(sf::Color(180, 180, 180));
    ui_panel.setOutlineThickness(2);
    window_.draw(ui_panel);

    sf::Text title(font_, "Game Stats", 24);
    title.setPosition(sf::Vector2f(ui_x, ui_y));
    title.setFillColor(sf::Color(60, 60, 80));
    window_.draw(title);

    std::ostringstream stats;
    stats << "\nLevel: " << game.GetCurrentLevel()
          << "\nHealth: " << player->GetHealth() << "/" << player->GetMaxHealth()
          << "\nScore: " << player->GetScore()
          << "\nEXP: " << player->GetExperiencePoints()
          << "\nPlayer Level: " << player->GetLevel()
          << "\nCombat Mode: " << (player->GetCombatMode() == CombatMode::MELEE ? "Melee" : "Ranged")
          << "\nDamage: " << player->GetDamage()
          << "\nEnemies Killed: " << game.GetEnemiesKilled();

    sf::Text stats_text(font_, stats.str(), 16);
    stats_text.setPosition(sf::Vector2f(ui_x, ui_y + 35));
    stats_text.setFillColor(sf::Color(60, 60, 80));
    window_.draw(stats_text);

    sf::Text spells_title(font_, "Spells (Click to Cast):", 20);
    spells_title.setPosition(sf::Vector2f(ui_x, ui_y + 240));
    spells_title.setFillColor(sf::Color(60, 60, 80));
    window_.draw(spells_title);

    for (int i = 0; i < hand->GetSpellCount(); ++i) {
        Spell* spell = hand->GetSpell(i);
        if (!spell) continue;

        int spell_y = ui_y + 270 + i * 60;

        sf::RectangleShape spell_box(sf::Vector2f(280, 50));
        spell_box.setPosition(sf::Vector2f(ui_x, spell_y));

        if (i == selected_spell_index_) {
            spell_box.setFillColor(sf::Color(200, 230, 255));
            spell_box.setOutlineColor(sf::Color(100, 150, 255));
        } else {
            spell_box.setFillColor(sf::Color(230, 240, 250));
            spell_box.setOutlineColor(sf::Color(150, 150, 150));
        }
        spell_box.setOutlineThickness(2);
        window_.draw(spell_box);

        std::ostringstream spell_info;
        spell_info << spell->GetName() << " (Lvl " << spell->GetLevel() << ")";

        sf::Text spell_text(font_, spell_info.str(), 14);
        spell_text.setPosition(sf::Vector2f(ui_x + 10, spell_y + 5));
        spell_text.setFillColor(sf::Color(40, 40, 60));
        window_.draw(spell_text);

        std::ostringstream spell_details;
        spell_details << "Range: " << spell->GetRange();

        sf::Text details_text(font_, spell_details.str(), 12);
        details_text.setPosition(sf::Vector2f(ui_x + 10, spell_y + 25));
        details_text.setFillColor(sf::Color(80, 80, 100));
        window_.draw(details_text);
    }

    sf::Text controls_title(font_, "\nControls:", 18);
    controls_title.setPosition(sf::Vector2f(ui_x, ui_y + 500));
    controls_title.setFillColor(sf::Color(60, 60, 80));
    window_.draw(controls_title);

    sf::Text controls(font_, "WASD - Move\nE - Attack\nC - Switch Combat Mode\nSpace - Skip Turn\nESC - Menu", 14);
    controls.setPosition(sf::Vector2f(ui_x, ui_y + 560));
    controls.setFillColor(sf::Color(60, 60, 80));
    window_.draw(controls);
}

void SfmlGameRenderer::DrawMenu(const Game& game) {
    sf::RectangleShape background(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    background.setFillColor(sf::Color(240, 245, 250));
    window_.draw(background);

    sf::Text title(font_, "Game", 48);
    title.setPosition(sf::Vector2f(200, 150));
    title.setFillColor(sf::Color(60, 80, 120));
    window_.draw(title);

    sf::Text instructions(font_, "Press ENTER to Start New Game\nPress L to Load Game\nPress ESC to Quit", 24);
    instructions.setPosition(sf::Vector2f(250, 350));
    instructions.setFillColor(sf::Color(80, 100, 140));
    window_.draw(instructions);
}

void SfmlGameRenderer::DrawPauseMenu(const Game& game) {
    sf::RectangleShape background(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    background.setFillColor(sf::Color(240, 245, 250));
    window_.draw(background);

    sf::Text title(font_, "Pause", 48);
    title.setPosition(sf::Vector2f(200, 150));
    title.setFillColor(sf::Color(60, 80, 120));
    window_.draw(title);

    sf::Text instructions(font_, 
        "Press ESC to Resume Game\nPress R to Restart Game\nPress S to Save Game\nPress L to Load Game\nPress Q to Quit", 
        24);
    instructions.setPosition(sf::Vector2f(250, 350));
    instructions.setFillColor(sf::Color(80, 100, 140));
    window_.draw(instructions);
}

void SfmlGameRenderer::DrawLevelComplete(const Game& game) {
    sf::RectangleShape overlay(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window_.draw(overlay);

    sf::RectangleShape panel(sf::Vector2f(500, 300));
    panel.setPosition(sf::Vector2f(250, 200));
    panel.setFillColor(sf::Color(220, 240, 255));
    panel.setOutlineColor(sf::Color(100, 150, 200));
    panel.setOutlineThickness(3);
    window_.draw(panel);

    sf::Text title(font_, "Level Complete!", 36);
    title.setPosition(sf::Vector2f(350, 240));
    title.setFillColor(sf::Color(40, 80, 120));
    window_.draw(title);

    std::ostringstream level_info;
    level_info << "Score: " << game.GetPlayer()->GetScore()
               << "\nEnemies Defeated: " << game.GetEnemiesKilled();

    sf::Text info(font_, level_info.str(), 20);
    info.setPosition(sf::Vector2f(320, 310));
    info.setFillColor(sf::Color(60, 100, 140));
    window_.draw(info);

    sf::Text instructions(font_, "Press ENTER to continue", 18);
    instructions.setPosition(sf::Vector2f(310, 420));
    instructions.setFillColor(sf::Color(80, 120, 160));
    window_.draw(instructions);
}

void SfmlGameRenderer::DrawUpgradeMenu(const Game& game) {
    sf::RectangleShape background(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    background.setFillColor(sf::Color(235, 245, 255));
    window_.draw(background);

    sf::Text title(font_, "Level Up! Choose an Upgrade:", 36);
    title.setPosition(sf::Vector2f(200, 100));
    title.setFillColor(sf::Color(60, 80, 120));
    window_.draw(title);

    const char* upgrades[] = {
        "1 - Increase Max Health (+20 HP)",
        "2 - Increase Damage (Melee +5, Ranged +3)",
        "3 - Gain Random Spell"
    };

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape button(sf::Vector2f(600, 60));
        button.setPosition(sf::Vector2f(200, 250 + i * 100));
        button.setFillColor(sf::Color(200, 220, 240));
        button.setOutlineColor(sf::Color(100, 140, 180));
        button.setOutlineThickness(2);
        window_.draw(button);

        sf::Text text(font_, upgrades[i], 20);
        text.setPosition(sf::Vector2f(220, 265 + i * 100));
        text.setFillColor(sf::Color(40, 60, 100));
        window_.draw(text);
    }
}

void SfmlGameRenderer::DrawGameOver(const Game& game) {
    sf::RectangleShape overlay(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window_.draw(overlay);

    sf::RectangleShape panel(sf::Vector2f(500, 300));
    panel.setPosition(sf::Vector2f(250, 200));
    panel.setFillColor(sf::Color(255, 230, 230));
    panel.setOutlineColor(sf::Color(200, 100, 100));
    panel.setOutlineThickness(3);
    window_.draw(panel);

    sf::Text title(font_, "Game Over", 48);
    title.setPosition(sf::Vector2f(330, 240));
    title.setFillColor(sf::Color(150, 50, 50));
    window_.draw(title);

    std::ostringstream stats;
    stats << "Final Score: " << game.GetPlayer()->GetScore()
          << "\nLevel Reached: " << game.GetCurrentLevel()
          << "\nEnemies Defeated: " << game.GetEnemiesKilled();

    sf::Text info(font_, stats.str(), 20);
    info.setPosition(sf::Vector2f(300, 320));
    info.setFillColor(sf::Color(100, 50, 50));
    window_.draw(info);

    sf::Text instructions(font_, "Press ENTER for new game\nPress L to Load Game\nPress ESC to quit", 18);
    instructions.setPosition(sf::Vector2f(290, 420));
    instructions.setFillColor(sf::Color(120, 60, 60));
    window_.draw(instructions);
}

void SfmlGameRenderer::Render(const Game& game) {
    window_.clear(sf::Color(245, 245, 240));

    switch (game.GetState()) {
        case GameState::MENU:
            DrawMenu(game);
            break;
        case GameState::PLAYING:
            DrawField(game);
            DrawEntities(game);
            DrawUI(game);
            break;
        case GameState::LEVEL_COMPLETE:
            DrawField(game);
            DrawEntities(game);
            DrawUI(game);
            DrawLevelComplete(game);
            break;
        case GameState::UPGRADE_MENU:
            DrawUpgradeMenu(game);
            break;
        case GameState::PAUSE_MENU:
            DrawPauseMenu(game);
            break;
        case GameState::GAME_OVER:
            DrawField(game);
            DrawEntities(game);
            DrawUI(game);
            DrawGameOver(game);
            break;
    }

    window_.display();
}

void SfmlGameRenderer::HandleClick(int mouse_x, int mouse_y, Game& game) {
    if (game.GetState() != GameState::PLAYING) return;

    const GameField* field = game.GetField();
    const PlayerHand* hand = game.GetPlayerHand();
    if (!field || !hand) return;

    int ui_x = offset_x_ + field->GetWidth() * cell_size_ + 30;
    int spell_start_y = offset_y_ + 270;

    if (mouse_x >= ui_x && mouse_x <= ui_x + 280) {
        for (int i = 0; i < hand->GetSpellCount(); ++i) {
            int spell_y = spell_start_y + i * 60;
            if (mouse_y >= spell_y && mouse_y <= spell_y + 50) {
                selected_spell_index_ = i;
                selecting_target_ = true;
                return;
            }
        }
    }

    if (selecting_target_ && selected_spell_index_ >= 0) {
        sf::Vector2i cell_pos = GetMouseCellPosition(mouse_x, mouse_y, game);
        if (cell_pos.x >= 0 && cell_pos.y >= 0) {
            Command cmd(PlayerAction::CAST_SPELL, selected_spell_index_, cell_pos.x, cell_pos.y);
            game.ProcessCommand(cmd);
            selected_spell_index_ = -1;
            selecting_target_ = false;
        }
    }
}

int SfmlGameRenderer::GetSelectedSpellIndex() const {
    return selected_spell_index_;
}

void SfmlGameRenderer::SetSelectedSpellIndex(int index) {
    selected_spell_index_ = index;
}

bool SfmlGameRenderer::IsSelectingTarget() const {
    return selecting_target_;
}

void SfmlGameRenderer::SetSelectingTarget(bool selecting) {
    selecting_target_ = selecting;
}

sf::Vector2i SfmlGameRenderer::GetMouseCellPosition(int mouse_x, int mouse_y, const Game& game) const {
    const GameField* field = game.GetField();
    if (!field) return sf::Vector2i(-1, -1);

    int cell_x = (mouse_x - offset_x_) / cell_size_;
    int cell_y = (mouse_y - offset_y_) / cell_size_;

    if (cell_x >= 0 && cell_x < field->GetWidth() && cell_y >= 0 && cell_y < field->GetHeight()) {
        return sf::Vector2i(cell_x, cell_y);
    }

    return sf::Vector2i(-1, -1);
}

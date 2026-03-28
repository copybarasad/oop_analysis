#include "SfmlRenderer.h"
#include "Game.h"

#include <iostream>
#include <sstream>

SfmlRenderer::SfmlRenderer()
    : cell_width_(64.f)
    , cell_height_(64.f)
    , panel_width_(220.f)
{
    if (!font_.loadFromFile("arial.ttf")) {
        std::cout << "Не удалось загрузить шрифт\n";
    }


    info_text_.setFont(font_);
    info_text_.setCharacterSize(18);
    info_text_.setFillColor(sf::Color::Black);
    info_text_.setPosition(5.f, 5.f);


    end_text_.setFont(font_);
    end_text_.setCharacterSize(96);
    end_text_.setFillColor(sf::Color::Red);
    end_text_.setString("");

    if (!hero_texture_.loadFromFile("pictures/maga.png")) {
        std::cout << "Не удалось загрузить изображение героя\n";
    }
    if (!enemy_texture_.loadFromFile("pictures/goblin.png")) {
        std::cout << "Не удалось загрузить изображение врага\n";
    }
    if (!base_texture_.loadFromFile("pictures/house.png")) {
        std::cout << "Не удалось загрузить изображение базы\n";
    }
    if (!brus_texture_.loadFromFile("pictures/grass.png")) {
        std::cout << "Не удалось загрузить изображение травы\n";
    }
    if (!wall_texture_.loadFromFile("pictures/wall.png")) {
        std::cout << "Не удалось загрузить изображение стены\n";
    }
    if (!water_texture_.loadFromFile("pictures/water.png")) {
        std::cout << "Не удалось загрузить изображение воды\n";
    }
    if (!trap_texture_.loadFromFile("pictures/Trap.png")) {
        std::cout << "Не удалось загрузить изображение ловушки\n";
    }
    if (!tower_texture_.loadFromFile("pictures/Tower.png")) {
        std::cout << "Не удалось загрузить изображение башни\n";
    }
    if (!ally_texture_.loadFromFile("pictures/ally.png")) {
        std::cout << "Не удалось загрузить изображение союзника\n";
    }

    hero_texture_.setSmooth(false);
    enemy_texture_.setSmooth(false);
    base_texture_.setSmooth(false);

    trap_texture_.setSmooth(false);
    tower_texture_.setSmooth(false);
    ally_texture_.setSmooth(false);

    brus_texture_.setSmooth(false);
    wall_texture_.setSmooth(false);
    water_texture_.setSmooth(false);


    hero_sprite_.setTexture(hero_texture_);
    enemy_sprite_.setTexture(enemy_texture_);
    base_sprite_.setTexture(base_texture_);

    trap_sprite_.setTexture(trap_texture_);
    tower_sprite_.setTexture(tower_texture_);
    ally_sprite_.setTexture(ally_texture_);

    brus_sprite_.setTexture(brus_texture_);
    wall_sprite_.setTexture(wall_texture_);
    water_sprite_.setTexture(water_texture_);
    sf::Vector2u h = hero_texture_.getSize();
    sf::Vector2u e = enemy_texture_.getSize();
    sf::Vector2u b = base_texture_.getSize();

    if (h.x != 0 && h.y != 0) {
        hero_sprite_.setScale(cell_width_ / static_cast<float>(h.x),
                              cell_height_ / static_cast<float>(h.y));
    }
    if (e.x != 0 && e.y != 0) {
        enemy_sprite_.setScale(cell_width_ / static_cast<float>(e.x),
                               cell_height_ / static_cast<float>(e.y));
    }
    if (b.x != 0 && b.y != 0) {
        base_sprite_.setScale(cell_width_ / static_cast<float>(b.x),
                              cell_height_ / static_cast<float>(b.y));
    }

    if (brus_texture_.getSize().x != 0 && brus_texture_.getSize().y != 0) {
        brus_sprite_.setScale(
            cell_width_ / static_cast<float>(brus_texture_.getSize().x),
            cell_height_ / static_cast<float>(brus_texture_.getSize().y));
    }
    if (wall_texture_.getSize().x != 0 && wall_texture_.getSize().y != 0) {
        wall_sprite_.setScale(
            cell_width_ / static_cast<float>(wall_texture_.getSize().x),
            cell_height_ / static_cast<float>(wall_texture_.getSize().y));
    }
    if (water_texture_.getSize().x != 0 && water_texture_.getSize().y != 0) {
        water_sprite_.setScale(
            cell_width_ / static_cast<float>(water_texture_.getSize().x),
            cell_height_ / static_cast<float>(water_texture_.getSize().y));
    }

    if (trap_texture_.getSize().x != 0 && trap_texture_.getSize().y != 0) {
        trap_sprite_.setScale(
            cell_width_ / static_cast<float>(trap_texture_.getSize().x),
            cell_height_ / static_cast<float>(trap_texture_.getSize().y));
    }
    if (tower_texture_.getSize().x != 0 && tower_texture_.getSize().y != 0) {
        tower_sprite_.setScale(
            cell_width_ / static_cast<float>(tower_texture_.getSize().x),
            cell_height_ / static_cast<float>(tower_texture_.getSize().y));
    }
    if (ally_texture_.getSize().x != 0 && ally_texture_.getSize().y != 0) {
        ally_sprite_.setScale(
            cell_width_ / static_cast<float>(ally_texture_.getSize().x),
            cell_height_ / static_cast<float>(ally_texture_.getSize().y));
    }

    panel_rect_.setFillColor(sf::Color(230, 230, 230));
}

void SfmlRenderer::Draw(sf::RenderWindow& window, Game& game){
    if (game.uiMode() == UiMode::MainMenu) {
        game.mainMenu().Draw(window, font_);
        return;
    }

    if (game.uiMode() == UiMode::Settings) {
        game.settingsMenu().Draw(window, font_);
        return;
    }

    if (game.uiMode() == UiMode::LevelUp) {
        game.levelUpMenu().Draw(window, font_);
        return;
    }

    Board& board = game.board();
    int boardWidth  = board.width();
    int boardHeight = board.height();

    const float cellW = game.cellWidth();
    const float cellH = game.cellHeight();
    const float panelW = game.panelWidth();

    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            Cell& c = board.at({x, y});
            sf::Sprite* tile = &brus_sprite_;

            if (!c.isPassable()) {
                tile = &wall_sprite_;
            } else if (c.isSlow()) {
                tile = &water_sprite_;
            }

            tile->setPosition(x * cellW, y * cellH);
            window.draw(*tile);
        }
    }

    EnemyBase* base = game.base();
    if (base != nullptr) {
        auto bp = base->position();
        base_sprite_.setPosition(bp.first * cellW, bp.second * cellH);
        window.draw(base_sprite_);
    }

    std::vector<Enemy>& enemies = game.enemies();
    for (Enemy& e : enemies) {
        if (!e.isAlive()) {
            continue;
        }
        auto ep = e.position();
        enemy_sprite_.setPosition(ep.first * cellW, ep.second * cellH);
        window.draw(enemy_sprite_);
    }

    std::vector<Ally>& allies = game.allies();
    for (Ally& a : allies) {
        if (!a.isAlive()) {
            continue;
        }
        auto ap = a.position();
        ally_sprite_.setPosition(ap.first * cellW, ap.second * cellH);
        window.draw(ally_sprite_);
    }

    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            Cell& c = board.at({x, y});
            if (c.hasTrap()) {
                trap_sprite_.setPosition(x * cellW, y * cellH);
                window.draw(trap_sprite_);
            }
        }
    }

    const std::vector<EnemyTower>& towers = game.towers();
    for (const EnemyTower& t : towers) {
        if (!t.isAlive()) {
            continue;
        }
        auto tp = t.position();
        tower_sprite_.setPosition(tp.first * cellW, tp.second * cellH);
        window.draw(tower_sprite_);
    }
    Player& player = game.player();
    auto pp = player.position();
    hero_sprite_.setPosition(pp.first * cellW, pp.second * cellH);
    window.draw(hero_sprite_);

    {
        std::stringstream ss;
        const LevelManager& lm = game.levelManager();

        ss << "Level: " << lm.CurrentLevel()
           << "/"      << lm.TotalLevels()
           << " HP: "  << player.hp()
           << " MODE: " << (player.isMode() ? "Ranged" : "Melee")
           << " Score: " << player.score()
           << " Enemies: " << enemies.size();

        info_text_.setString(ss.str());
        window.draw(info_text_);
    }

    panel_rect_.setSize(sf::Vector2f(panelW, boardHeight * cellH));
    panel_rect_.setPosition(boardWidth * cellW, 0.f);
    window.draw(panel_rect_);

    panel_lines_.clear();

    std::vector<const char*> names;
    player.hand().GetCardNames(names);

    for (std::size_t i = 0; i < names.size(); ++i) {
        sf::Text t;
        t.setFont(font_);
        t.setCharacterSize(18);
        t.setString(names[i]);
        t.setFillColor(
            static_cast<int>(i) == player.hand().GetSelectedIndex()
            ? sf::Color::Blue
            : sf::Color::Black
        );
        float x = boardWidth * cellW + 16.f;
        float y = 20.f + static_cast<float>(i) * 26.f;
        t.setPosition(x, y);
        panel_lines_.push_back(t);
    }

    for (const sf::Text& line : panel_lines_) {
        window.draw(line);
    }

    if (game.state() != Game::GameState::Running) {
        if (game.state() == Game::GameState::Defeat) {
            end_text_.setString("GAME OVER");
            end_text_.setFillColor(sf::Color::Red);
        } else if (game.state() == Game::GameState::Victory) {
            end_text_.setString("VICTORY");
            end_text_.setFillColor(sf::Color::Green);
        }

        sf::FloatRect bounds = end_text_.getLocalBounds();
        float cx = (boardWidth * cellW) / 2.f;
        float cy = (boardHeight * cellH) / 2.f;

        end_text_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        end_text_.setPosition(cx, cy);

        window.draw(end_text_);
    }
}
    
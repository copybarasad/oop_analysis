#include "GameView.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

const std::string SPELLS_TEXTURES_TO_LOAD[] = {
    INSTANT_DAMAGE_SPELL,
    HEALTH_SPELL,
    SPLASH_DAMAGE_SPELL,
    TRAP_SPELL,
    BOOST_SPELL,
    ALLY_SPELL
};

GameView::GameView(sf::RenderWindow& window, GameModel& model)
    : window_(window)
    , model_(model) {
    loadSprite("crystalKing", "assets/crystalKing/crystal_mauler_free_288x128_SpriteSheet.png", 288, 128, 288*2, 128*2);
    loadAnimation("crystalKing-idle", 0, 7, 0);
    loadAnimation("crystalKing-moving", 0, 7, 1);
    loadAnimation("crystalKing-attacking", 0, 7, 5);
    loadAnimation("crystalKing-attacking-range", 0, 14, 10);
    loadSprite("fireKing", "assets/fireKing/spritesheet.png", 288, 128, 288*2, 128*2);
    loadAnimation("fireKing-idle", 0, 7, 0);
    loadAnimation("fireKing-moving", 0, 7, 1);
    loadAnimation("fireKing-attacking", 0, 9, 7);

    view_.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    view_.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

    ui_view_.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    ui_view_.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

    if(!tileset_texture_.loadFromFile("assets/tileset.png")) {
        throw std::runtime_error("Failed to load tileset texture: assets/tileset.png");
    }

    if(!ui_tileset_texture_.loadFromFile("assets/ui.png")) {
        throw std::runtime_error("Failed to load tileset texture: assets/ui.png");
    }

    for(auto spell : SPELLS_TEXTURES_TO_LOAD) {
        textures_repository_.insert(std::make_pair(spell, sf::Texture()));
        std::string file_path = "assets/spells/" + spell + ".png";
        if(!textures_repository_[spell].loadFromFile(file_path)) {
            throw std::runtime_error("Failed to load texture: " + file_path);
        }
    }

    if(!pressStartFont_.openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        throw std::runtime_error("Failed to load font: assets/fonts/PressStart2P-Regular.ttf");
    }
}

sf::Vector2f GameView::calculateScale(int w1, int h1, int w2, int h2) {
    return sf::Vector2f(w2 * 1.0f / w1, h2 * 1.0f / h1);
}

void GameView::loadAnimation(const std::string& logical_name, unsigned int start_x, unsigned int end_x, unsigned int y) {
    AnimationInfo info;
    info.start_x = start_x;
    info.end_x = end_x;
    info.y = y;
    animation_repository_[logical_name] = std::move(info);
}

void GameView::loadSprite(const std::string& logical_name, const std::string& file_path, unsigned int fw, unsigned int fh, unsigned int dw, unsigned int dh) {
    SpriteInfo info;
    if (!info.texture.loadFromFile(file_path)) {
        throw std::runtime_error("Failed to load texture: " + file_path);
    }
    info.frame_width = fw;
    info.frame_height = fh;
    info.display_width = dw;
    info.display_height = dh;
    sprite_repository_[logical_name] = std::move(info);
}

std::string GameView::getAnimationNameForEntity(const Entity* entity) {
    std::string animation_name = entity->getTextureName() + "-" + stateToString(entity->getState());
    if(entity->getState() == State::Attacking && entity->isRangeAttack()) {
        animation_name += "-range";
    }
    return animation_name;
}

void GameView::updateAnimationState(const Entity* entity, float dt) {
    AnimationInfo info = animation_repository_.at(getAnimationNameForEntity(entity));
    if (animation_state_repository_.find(entity) == animation_state_repository_.end()) {
        EntityAnimationState state;
        state.current_frame = 0;
        state.timer = 0.0f;
        animation_state_repository_[entity] = std::move(state);
    }
    EntityAnimationState& animation_state = animation_state_repository_.at(entity);

    std::string current_anim = getAnimationNameForEntity(entity);
    if(current_anim != animation_state.animation_name) {
        info = animation_repository_.at(current_anim);
        animation_state.animation_name = current_anim;
        animation_state.current_frame = 0;
        animation_state.timer = 0.0f;
    }

    animation_state.timer += dt;
    if(animation_state.timer >= animation_state.speed) {
        animation_state.current_frame = (animation_state.current_frame + 1) % (info.end_x - info.start_x + 1);
        animation_state.timer = 0.0f;
    }
    
}

void GameView::drawEntity(const Entity* e) {
    AnimationInfo info = animation_repository_.at(getAnimationNameForEntity(e));
    EntityAnimationState& animation_state = animation_state_repository_.at(e);
    const SpriteInfo& sprite_info = sprite_repository_.at(e->getTextureName());
    sf::Sprite sprite(sprite_info.texture);
    sf::Vector2f scale = calculateScale(sprite_info.frame_width, sprite_info.frame_height, sprite_info.display_width, sprite_info.display_height);
    scale.x *= e->getDirection();
    sprite.setTextureRect(sf::IntRect(sf::Vector2i((animation_state.current_frame + info.start_x) * sprite_info.frame_width, info.y * sprite_info.frame_height), sf::Vector2i(sprite_info.frame_width, sprite_info.frame_height)));
    sprite.setPosition(sf::Vector2f(e->getX()*cell_size_ + 30.0f - (float(sprite_info.frame_width) / 2.0f) * scale.x, e->getY()*cell_size_ - ((float(sprite_info.frame_height) / 2.0f) + 45.0f) * scale.y));
    sprite.setScale(scale);
    window_.draw(sprite); 

    sf::RectangleShape health_bar_background(sf::Vector2f(cell_size_*0.9f, 10.0f));
    health_bar_background.setFillColor(sf::Color::White);
    health_bar_background.setPosition(sf::Vector2f(e->getX()*cell_size_ + cell_size_*0.05f, (e->getY() + 1)*cell_size_ - 20.0f));
    window_.draw(health_bar_background);
    sf::RectangleShape health_bar(sf::Vector2f(cell_size_*0.9f, 10.0f));
    health_bar.setFillColor(sf::Color::Red);
    health_bar.setPosition(sf::Vector2f(e->getX()*cell_size_ + cell_size_*0.05f, (e->getY() + 1)*cell_size_ - 20.0f));
    health_bar.setSize(sf::Vector2f(cell_size_*0.9f * (e->getHealth() * 1.0f / e->getMaxHealth()), 10.0f));
    window_.draw(health_bar);
}

void GameView::drawField() {
    sf::Sprite tileset_sprite_(tileset_texture_);
    const GameField& field = model_.getField();
    for(int y = 0; y < field.getHeight(); y++) {
        for(int x = 0; x < field.getWidth(); x++) {
            char cell = field.getCell(x, y);
            int tile_x, tile_y;
            switch(cell) {
                case TILE_WALL:
                    tile_x = 1;
                    tile_y = 0;
                    break;
                case TILE_PLAYER_TRAP:
                    tile_x = 0;
                    tile_y = 17;
                    break;
                default:
                    tile_x = 0;
                    tile_y = 1;
                    break;
            }
            tileset_sprite_.setTextureRect(sf::IntRect(sf::Vector2i(tile_x * 16, tile_y * 16), sf::Vector2i(16, 16)));
            tileset_sprite_.setScale(calculateScale(16, 16, cell_size_, cell_size_));
            tileset_sprite_.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_));
            window_.draw(tileset_sprite_);
            if(field.getTrap(x, y)) {
                tile_x = 1;
                tile_y = 17;
                tileset_sprite_.setTextureRect(sf::IntRect(sf::Vector2i(tile_x * 16, tile_y * 16), sf::Vector2i(16, 16)));
                tileset_sprite_.setScale(calculateScale(16, 16, cell_size_, cell_size_));
                tileset_sprite_.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_));
                window_.draw(tileset_sprite_);
            }

            sf::Color color = field.getColorAt(x, y);
            sf::RectangleShape rectangle(sf::Vector2f(cell_size_, cell_size_));
            rectangle.setFillColor(color);
            rectangle.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_));
            window_.draw(rectangle);
        }
    }
}

void GameView::drawUIElement(float x, float y, int cx, int cy, int w, int h, float dw, float dh) {
    sf::Vector2f scale = (dw != 0 && dh != 0) ? calculateScale(w * 16, h * 16, dw, dh) : sf::Vector2f(1.0f, 1.0f);
    sf::Sprite ui(ui_tileset_texture_);
    ui.setTextureRect(sf::IntRect(sf::Vector2i(cx * 16, cy * 16), sf::Vector2i(w * 16, h * 16)));
    ui.setScale(scale);
    ui.setPosition(sf::Vector2f(x, y));
    window_.draw(ui);
}

void GameView::drawSelectedCell() {
    if(!model_.getCanPlayerMove()) return;
    std::pair<int, int> selected_cell = model_.getSelectedCell();
    int anim_shift = ui_timer_ * 2 >= ui_max_timer_ ? 8 : 0;
    if(selected_cell.first >= 0 && selected_cell.second >= 0 && selected_cell.first < model_.getField().getWidth() && selected_cell.second < model_.getField().getHeight()) {
        drawUIElement(selected_cell.first * cell_size_ - 32 - anim_shift, selected_cell.second * cell_size_ - 32 - anim_shift, 5, 4, 1, 1, 64, 64);
        drawUIElement(selected_cell.first * cell_size_ + 32 + cell_size_  - 64 + anim_shift, selected_cell.second * cell_size_ - 32 - anim_shift, 6, 4, 1, 1, 64, 64);
        drawUIElement(selected_cell.first * cell_size_ - 32 - anim_shift, selected_cell.second * cell_size_ + 32 + cell_size_ - 64 + anim_shift, 5, 5, 1, 1, 64, 64);
        drawUIElement(selected_cell.first * cell_size_ + 32 + cell_size_ - 64 + anim_shift, selected_cell.second * cell_size_ + 32 + cell_size_ - 64 + anim_shift, 6, 5, 1, 1, 64, 64);
    }
}

void GameView::updateView() {
    const Entity* player = model_.getPlayer();
    if(!player) return;
    view_.setCenter(sf::Vector2f(player->getX()*cell_size_ - 30.0f, player->getY()*cell_size_ - 30.0f));
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
    sf::Vector2f mouse_world_pos = window_.mapPixelToCoords(mouse_pos);
    int x = mouse_world_pos.x / cell_size_;
    int y = mouse_world_pos.y / cell_size_;
    model_.selectCell(x, y);
}

void GameView::updateUITimer(float dt) {
    ui_timer_ += dt;
    while(ui_timer_ > ui_max_timer_) {
        ui_timer_ -= ui_max_timer_;
    }
}

void GameView::drawUI(const Player* player) {
    sf::Font font;
    font.openFromFile("assets/fonts/PressStart2P-Regular.ttf");
    std::string text_str = model_.getCanPlayerMove() ? "Your turn" : "Enemy turn";
    sf::Text text(font, text_str);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10.0f, 10.0f));
    window_.draw(text);
    text_str = "Health: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 40.0f));
    window_.draw(text);
    text_str = "Attack mode: " + (std::string)(player->isRangeAttack() ? "range" : "melee");
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 70.0f));
    window_.draw(text);
    text_str = "Balance: " + std::to_string(model_.getWallet().getBalance());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 100.0f));
    window_.draw(text);
    text_str = "Boost level: " + std::to_string(model_.getBoostLevel());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 130.0f));
    window_.draw(text);

    text_str = "Current score: " + std::to_string(model_.getScore());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 160.0f));
    window_.draw(text);

    text_str = "Goal: " + std::to_string(model_.getGoal());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 190.0f));
    window_.draw(text);

    text_str = "Game level: " + std::to_string(model_.getGameLevel());
    text.setString(text_str);
    text.setPosition(sf::Vector2f(10.0f, 220.0f));
    window_.draw(text);
    
    // Show save error message if there is one
    if (!model_.getSaveGameErrorMessage().empty()) {
        text_str = "Save Error: " + model_.getSaveGameErrorMessage();
        text.setString(text_str);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Red);
        text.setPosition(sf::Vector2f(10.0f, 310.0f));
        window_.draw(text);
    }

    drawDeck();
    drawShop();
}

void GameView::drawBuildings() {
    sf::Sprite tileset_sprite_(tileset_texture_);
    const GameField& field = model_.getField();

    for(int y = 0; y < field.getHeight(); y++) {
        for(int x = 0; x < field.getWidth(); x++) {
            SpawnerBuilding* b = field.getSpawnerBuildingAt(x, y);
            if(b != nullptr) {
                int tile_x = 0, tile_y = 2;
                tileset_sprite_.setTextureRect(sf::IntRect(sf::Vector2i(tile_x * 16, tile_y * 16), sf::Vector2i(16*2, 16*2)));
                tileset_sprite_.setScale(calculateScale(32, 32, cell_size_, cell_size_));
                tileset_sprite_.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_));
                window_.draw(tileset_sprite_);
            }

            Tower* t = field.getTowerAt(x, y);
            if(t != nullptr) {
                int tile_x = 5, tile_y = 10;
                tileset_sprite_.setTextureRect(sf::IntRect(sf::Vector2i(tile_x * 16, tile_y * 16), sf::Vector2i(16, 16*2)));
                tileset_sprite_.setScale(calculateScale(16, 32, cell_size_, cell_size_));
                tileset_sprite_.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_));
                window_.draw(tileset_sprite_);
                int health = t->getHealth(), max_health = t->getMaxHealth();
                sf::RectangleShape health_bar_back(sf::Vector2f(cell_size_, 4));
                health_bar_back.setFillColor(sf::Color::White);
                health_bar_back.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_ + cell_size_ - 4));
                window_.draw(health_bar_back);
                sf::RectangleShape health_bar(sf::Vector2f(cell_size_ * health / max_health, 4));
                health_bar.setFillColor(sf::Color::Red);
                health_bar.setPosition(sf::Vector2f(x*cell_size_, y*cell_size_ + cell_size_ - 4));
                window_.draw(health_bar);
            }
        }
    }
}

void GameView::drawEndGame() {
    const std::vector<ButtonModel> btns = model_.getMenuButtons();

    model_.setSelectedBtnId(0);

    sf::Text title(pressStartFont_, "Game over");
    title.setCharacterSize(40);
    title.setLineSpacing(2.0f);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(window_.getSize().x / 2 - 100, 50.0f));
    window_.draw(title);

    sf::Text errorText(pressStartFont_, model_.getSaveGameErrorMessage());
    errorText.setCharacterSize(20);
    errorText.setLineSpacing(1.0f);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(sf::Vector2f(20, window_.getSize().y - 100));
    window_.draw(errorText);

    for(size_t i = 0; i < btns.size(); ++i) {
        drawBtn(
            btns[i],
            window_.getSize().x / 2 - 300,
            200 + 120 * i
        );
    }
}

void GameView::drawSpell(
    Spell* spell,
    float x,
    float y,
    float dw,
    float dh
) {
    if(!spell) return;
    if(dw <= 0) dw = spell_size_;
    if(dh <= 0) dh = spell_size_;
    std::string name = spellTypeToString(spell->getName());
    if(!textures_repository_.count(name)) {
        std::cout << "Texture not found: " << name << std::endl;
        return;
    }


    sf::Sprite sprite(textures_repository_[name]);
    sf::Vector2f scale = calculateScale(spell_size_, spell_size_, dw, dh);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(scale);
    window_.draw(sprite);

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
    sf::Vector2f mouse_world_pos = window_.mapPixelToCoords(mouse_pos);

    bool isMouseOver = sprite.getGlobalBounds().contains(mouse_world_pos);

    if(isMouseOver || model_.getSelectedSpell() == spell) {
        sf::RectangleShape rect(sf::Vector2f(dw, dh));
        rect.setFillColor(sf::Color(255, 255, 255, 50));
        rect.setPosition(sf::Vector2f(x, y));
        window_.draw(rect);
    }

    if(isMouseOver) {
        model_.setMouseOnSpell(spell);
    }
}

void GameView::drawDeck() {
    const Deck& deck = model_.getDeck();
    sf::RectangleShape bg(sf::Vector2f((spell_draw_size_ + 10.0f) * deck.getSize() + 20.0f, spell_draw_size_ + 10.0f));
    bg.setFillColor(sf::Color(55, 55, 55, 200));
    bg.setPosition(sf::Vector2f(0, window_.getSize().y - spell_draw_size_ - 10.0f));
    window_.draw(bg);

    model_.setMouseOnSpell(nullptr);

    for(size_t i = 0; i < deck.getSize(); ++i) {
        Spell* spell = deck.getSpell(i);
        if(spell) {
            drawSpell(
                spell,
                10.0f + i * (spell_draw_size_ + 10.0f),
                window_.getSize().y - spell_draw_size_ - 5.0f,
                spell_draw_size_,
                spell_draw_size_
            );
        } else {
            sf::RectangleShape rect(sf::Vector2f(spell_draw_size_, spell_draw_size_));
            rect.setFillColor(sf::Color(155, 155, 155, 128));
            rect.setPosition(sf::Vector2f(10.0f + i * (spell_draw_size_ + 10.0f), window_.getSize().y - spell_draw_size_ - 5.0f));
            window_.draw(rect);
        }
    }
}

void GameView::drawBtn(const ButtonModel& btn, int x, int y) {
    sf::RectangleShape bg(sf::Vector2f(600.0f, 100.0f));
    bg.setFillColor(sf::Color(70, 70, 70, 128));
    bg.setPosition(sf::Vector2f(x, y));
    window_.draw(bg);

    sf::Text title(pressStartFont_, btn.getTitle());
    title.setCharacterSize(20);
    title.setLineSpacing(2.0f);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(x + 20.0f, y + 10.0f));
    window_.draw(title);

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
    sf::Vector2f mouse_world_pos = window_.mapPixelToCoords(mouse_pos);

    if(bg.getGlobalBounds().contains(mouse_world_pos)) {
        sf::RectangleShape rect(sf::Vector2f(600.0f, 100.0f));
        rect.setFillColor(sf::Color(255, 255, 255, 50));
        rect.setPosition(sf::Vector2f(x, y));
        window_.draw(rect);
        model_.setSelectedBtnId(btn.getId());
    }
}

void GameView::drawMenu() {
    const std::vector<ButtonModel> btns = model_.getMenuButtons();

    model_.setSelectedBtnId(0);

    sf::Text title(pressStartFont_, "Super Game");
    title.setCharacterSize(40);
    title.setLineSpacing(2.0f);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(window_.getSize().x / 2 - 100, 50.0f));
    window_.draw(title);

    sf::Text errorText(pressStartFont_, model_.getSaveGameErrorMessage());
    errorText.setCharacterSize(20);
    errorText.setLineSpacing(1.0f);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(sf::Vector2f(20, window_.getSize().y - 100));
    window_.draw(errorText);

    for(size_t i = 0; i < btns.size(); ++i) {
        drawBtn(
            btns[i],
            window_.getSize().x / 2 - 300,
            200 + 120 * i
        );
    }
}

void GameView::drawNextLevel() {
    const std::vector<ButtonModel> btns = model_.getNextLevelButtons();

    model_.setSelectedBtnId(0);

    sf::Text title(pressStartFont_, "Next level");
    title.setCharacterSize(40);
    title.setLineSpacing(2.0f);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(window_.getSize().x / 2 - 100, 50.0f));
    window_.draw(title);

    title.setString("Balance: " + std::to_string(model_.getWallet().getBalance()));
    title.setCharacterSize(20);
    title.setPosition(sf::Vector2f(50, 50));
    window_.draw(title);

    for(size_t i = 0; i < btns.size(); ++i) {
        drawBtn(
            btns[i],
            window_.getSize().x / 2 - 300,
            200 + 120 * i
        );
    }
}

void GameView::drawShopItem(
    const ShopItem* item,
    float x,
    float y
) {
    if(!item) return;
    std::string name = spellTypeToString(item->getSpell()->getName());
    if(!textures_repository_.count(name)) {
        std::cout << "Texture not found: " << name << std::endl;
        return;
    }

    sf::RectangleShape bg(sf::Vector2f(300.0f, spell_draw_size_));
    bg.setFillColor(sf::Color(70, 70, 70, 128));
    bg.setPosition(sf::Vector2f(x, y));
    window_.draw(bg);

    sf::Sprite sprite(textures_repository_[name]);
    sf::Vector2f scale = calculateScale(spell_size_, spell_size_, spell_draw_size_, spell_draw_size_);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(scale);
    window_.draw(sprite);

    sf::Text title(pressStartFont_, item->getTitle());
    title.setCharacterSize(12);
    title.setLineSpacing(2.0f);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(x + spell_draw_size_ + 20.0f, y + 10.0f));
    window_.draw(title);

    sf::Text description(pressStartFont_, item->getDescription());
    description.setCharacterSize(8);
    description.setLineSpacing(1.5f);
    description.setFillColor(sf::Color(255, 255, 255, 128));
    description.setPosition(sf::Vector2f(x + spell_draw_size_ + 20.0f, y + 30.0f));
    window_.draw(description);

    sf::Text price(pressStartFont_, std::to_string(item->getCost()));
    price.setCharacterSize(16);
    price.setFillColor(sf::Color::White);
    price.setPosition(sf::Vector2f(x + spell_draw_size_ + 20.0f, y + 60.0f));
    window_.draw(price);

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
    sf::Vector2f mouse_world_pos = window_.mapPixelToCoords(mouse_pos);

    if(bg.getGlobalBounds().contains(mouse_world_pos)) {
        sf::RectangleShape rect(sf::Vector2f(300.0f, spell_draw_size_));
        rect.setFillColor(sf::Color(255, 255, 255, 50));
        rect.setPosition(sf::Vector2f(x, y));
        window_.draw(rect);
        model_.selectShopItem(item);
    }
}

void GameView::drawShop() {

    const Shop& shop = model_.getShop();

    sf::RectangleShape bg(sf::Vector2f(300.0f, shop.getSize() * spell_draw_size_ + 105.0f));
    bg.setFillColor(sf::Color(55, 55, 55, 128));
    bg.setPosition(sf::Vector2f(window_.getSize().x - 300.0f, 0.0f));
    window_.draw(bg);

    std::string text_str = "Shop";
    sf::Text text(pressStartFont_, text_str);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(window_.getSize().x - 200.0f, 10.0f));
    window_.draw(text);

    model_.selectShopItem(nullptr);

    for(size_t i = 0; i < shop.getSize(); ++i) {
        drawShopItem(shop.getItem(i), window_.getSize().x - 300.0f, 50.0f + i * spell_draw_size_);
    }
}

void GameView::render(float dt) {
    window_.clear(sf::Color::Black);

    if(model_.getGameState() == GameState::Menu) {
        window_.setView(ui_view_);
        drawMenu();
    }
    else if(model_.getGameState() == GameState::Next) {
        window_.setView(ui_view_);
        drawNextLevel();
    } else {
        window_.setView(view_);
        updateView();
        updateUITimer(dt);

        drawField();
        drawBuildings();
        std::vector<const Entity*> entities = model_.getEntities();
        std::sort(entities.begin(), entities.end(), [](const Entity* a, const Entity* b) {
            return a->getY() < b->getY();
        });
        for(const Entity* e : entities) {
            updateAnimationState(e, dt);
            drawEntity(e);
        }

        if(model_.getGameState() == GameState::GameOver) {
            window_.setView(ui_view_);

            drawEndGame();
        } else {
            drawSelectedCell();

            window_.setView(ui_view_);

            if(model_.getPlayer())
                drawUI(model_.getPlayer());
        }

    }


    window_.display();
}
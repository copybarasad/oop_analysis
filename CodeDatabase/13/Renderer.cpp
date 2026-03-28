#include "Renderer.h"
#include <map>

#include "../basics/Object.h"
#include "../basics/Entity.h"
#include "../Entities/Player.h"
#include "../Environment/SpellTower.h"

void Renderer::run() {
    font_.loadFromFile("Renderer/graphics/fonts/Ramona.ttf");
    float diff = this->sprite_size_ / 80;
    prepareText(this->hpText_);
    prepareText(this->scoreText_);
    prepareText(this->spellLevel_);
    this->spellLevel_.setFillColor(sf::Color::Magenta);
    prepareText(this->spellName_);
    this->spellName_.setPosition(80 * diff, 40 * diff);
    prepareText(this->spellDesc_);
    this->spellDesc_.setPosition(80 * diff, 100 * diff);
    prepareText(this->overText_, 128);
    this->load_sprites();
    this->sprites_map_.at("Paper").setScale(diff * 1.5, diff * 0.5);
    this->sprites_map_.at("Frame").setScale(diff, diff);
    this->sprites_map_.at("Mana0").setScale(diff, diff);
    this->sprites_map_.at("Mana1").setScale(diff, diff);
    this->sprites_map_.at("Mana2").setScale(diff, diff);
    this->sprites_map_.at("Mana3").setScale(diff, diff);
    this->sprites_map_.at("Mana4").setScale(diff, diff);
    this->sprites_map_.at("Mana5").setScale(diff, diff);
    this->sprites_map_.at("NeedMana1").setScale(diff, diff);
    this->sprites_map_.at("NeedMana2").setScale(diff, diff);
    this->sprites_map_.at("NeedMana3").setScale(diff, diff);
    this->sprites_map_.at("NeedMana4").setScale(diff, diff);
    this->sprites_map_.at("NeedMana5").setScale(diff, diff);

    this->map_size_ = this->glogic_->getMap().getSize();
    window_.create(sf::VideoMode(22 * this->sprite_size_,
                                 12 * this->sprite_size_), this->game_name_);
    this->updateMapSize();

    this->hpText_.setPosition(19.1 * this->sprite_size_, 10 * diff);
    this->scoreText_.setPosition(19.1 * this->sprite_size_, 50 * diff);
    this->spellLevel_.setPosition(21.2 * this->sprite_size_, 200 * diff);
    this->overText_.setPosition(19 * this->sprite_size_ / 2 - 300 * diff,
                                12 * this->sprite_size_ / 2 - 100 * diff);
    this->setStatusText(0, 0);
    this->overText_.setString("You're cooked");

    this->fadeRect_.setSize(sf::Vector2f(22 * this->sprite_size_,
                                         12 * this->sprite_size_));
    this->fadeRect_.setFillColor(sf::Color(0, 0, 0, 140));
    this->spellBar_.setSize(sf::Vector2f(3 * this->sprite_size_,
                                         12 * this->sprite_size_));
    this->spellBar_.setFillColor(sf::Color(188, 144, 88));
    this->spellBar_.setPosition(19 * this->sprite_size_, 0);
}

void Renderer::close() {
    window_.close();
}

bool Renderer::pollEvent(sf::Event &event) {
    return window_.pollEvent(event);
}

sf::Sprite Renderer::prepareSprite(const std::string &name,
                                   const int x, const int y, const bool weapon, bool need_offset) {
    sf::Sprite sprite = this->getSprite(name, weapon);
    sprite.setPosition(this->map_offset_x_ * need_offset + x * this->sprite_size_,
                       this->map_offset_y_ * need_offset + y * this->sprite_size_);
    return sprite;
}


void Renderer::renderGame() {
    this->updateMapSize();
    this->window_.clear(this->background_);
    window_.draw(this->spellBar_);

    for (size_t y = 0; y < this->map_size_.y_; y++) {
        for (size_t x = 0; x < this->map_size_.x_; x++) {
            this->window_.draw(this->prepareSprite("Cell", x, y));

            for (size_t layer = 0; layer < this->map_size_.layer_; layer++) {
                if (this->map_->isOccupied(Coordinates(x, y, layer))) {
                    Object *obj = this->map_->getFromCell(Coordinates(x, y, layer));
                    this->window_.draw(this->prepareSprite(obj->getName(), x, y));

                    Entity *entityPtr = dynamic_cast<Entity *>(obj);
                    if (entityPtr) {
                        std::string weapon = entityPtr->getWeapon()->name_;
                        this->window_.draw(this->prepareSprite(weapon, x, y, true));
                        if (entityPtr->getStun()) {
                            this->window_.draw(this->prepareSprite("stun", x, y));
                        }
                    } else {
                        SpellTower *spt = dynamic_cast<SpellTower *>(obj);
                        if (spt && spt->isReady()) {
                            sf::Sprite tower = this->prepareSprite("SpellTower", x, y);
                            tower.setColor(sf::Color(255, 255, 0));
                            this->window_.draw(tower);
                        }
                    }
                }
            }
        }
    }

    window_.draw(this->hpText_);
    window_.draw(this->scoreText_);

    auto spellBook = this->glogic_->getPlayer()->getSpellBook();
    this->spellLevel_.setString(std::to_string(spellBook->getLevel()));
    window_.draw(this->spellLevel_);
    if (this->glogic_->getPlayer()->isBookOpen() && spellBook->haveSpells()) {
        this->window_.draw(prepareSprite("Frame", 19,
                                         spellBook->getChosenSpellId() + 3, false, false));
        this->window_.draw(prepareSprite(
            "NeedMana" + std::to_string(std::get<3>(spellBook->getSpells().at(spellBook->getChosenSpellId()))),
            19, 1, false, false));
        if (this->showSpellInfo_) {
            this->window_.draw(this->prepareSprite("Paper", 0, 0, false, false));
            this->spellName_.setString(
                std::get<0>(spellBook->getSpells().at(spellBook->getChosenSpellId())));
            this->window_.draw(this->spellName_);
            this->spellDesc_.setString(
                std::get<1>(spellBook->getSpells().at(spellBook->getChosenSpellId())));
            this->window_.draw(this->spellDesc_);
        }
    }

    sf::Text quantity;
    this->prepareText(quantity);
    int yoffset = 3;
    for (auto i: spellBook->getSpells()) {
        if (spellBook->getLevel()) {
            this->window_.draw(prepareSprite(std::get<0>(i) + " advance", 19, yoffset++, false, false));
        } else {
            this->window_.draw(prepareSprite(std::get<0>(i), 19, yoffset++, false, false));
        }
        quantity.setString(" X  " + std::to_string(std::get<2>(i)));
        quantity.setPosition(20 * this->sprite_size_,
                             (yoffset - 0.5) * this->sprite_size_);
        this->window_.draw(quantity);
    }
    this->window_.draw(prepareSprite("Mana" + std::to_string(spellBook->getMana()),
                                     19, 1, false, false));
    this->setStatusText(this->glogic_->getPlayer()->getHp(), this->glogic_->getScore());


    if (this->isFade) {
        window_.draw(this->fadeRect_);
    }

    if (this->isOver) {
        window_.draw(this->overText_);
    }

    this->window_.display();
}

sf::RenderWindow *Renderer::getWindow() {
    return &this->window_;
}


void Renderer::load_sprites() {
    for (auto pair: this->files_map_) {
        if (sf::Texture texture; texture.loadFromFile("Renderer/graphics/images/" + pair.second)) {
            this->texstures_map_[pair.first] = texture;
            this->sprites_map_[pair.first] = sf::Sprite(this->texstures_map_[pair.first]);
            const sf::Vector2u size = texture.getSize();
            this->sprites_map_[pair.first].setScale(sprite_size_ / size.x, sprite_size_ / size.y);
        }
    }
}

sf::Sprite &Renderer::getSprite(const std::string &name, const bool weapon) {
    if (this->sprites_map_.contains(name)) {
        return this->sprites_map_[name];
    }
    if (weapon) {
        return this->sprites_map_["weapon_placeholder"];
    }
    return this->sprites_map_["placeholder"];
}

void Renderer::prepareText(sf::Text &text, const int size) const {
    text.setFont(this->font_);
    text.setCharacterSize(size * this->sprite_size_ / 80);
    text.setFillColor(sf::Color::White);
}

void Renderer::setStatusText(const int hp, const int score) {
    this->hpText_.setString("Hp: " + std::to_string(hp));
    this->scoreText_.setString("Score: " + std::to_string(score));
}

void Renderer::switchFade(const bool sw) {
    this->isFade = sw;
}

void Renderer::switchOver(const bool sw) {
    this->switchFade(sw);
    this->isOver = sw;
}

void Renderer::switchSpellInfoState() {
    this->showSpellInfo_ = !this->showSpellInfo_;
}

void Renderer::updateMapSize() {
    this->map_size_ = this->glogic_->getMap().getSize();
    this->map_offset_x_ = static_cast<float>(19 - this->map_size_.x_) * this->sprite_size_ / 2;
    this->map_offset_y_ = static_cast<float>(12 - this->map_size_.y_) * this->sprite_size_ / 2;
}


int Renderer::renderMenu(const std::vector<std::string> &options) {
    if (options.empty()) {
        return -1;
    }

    std::size_t selected = 0;

    while (this->window_.isOpen()) {
        sf::Event event{};
        while (this->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->close();
                return -1;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    this->close();
                    return -1;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    if (selected == 0) {
                        selected = options.size() - 1;
                    } else {
                        selected--;
                    }
                }
                if (event.key.code == sf::Keyboard::Down) {
                    selected = (selected + 1) % options.size();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    return static_cast<int>(selected);
                }
            }
        }

        this->window_.clear(this->background_);

        float diff = this->sprite_size_ / 80.f;

        float baseX = 21.5f * this->sprite_size_ / 2.f;
        float baseY = 11.f * this->sprite_size_ / 2.f;
        float stepY = 80.f * diff;

        for (std::size_t i = 0; i < options.size(); ++i) {
            sf::Text text;
            text.setFont(this->font_);
            text.setCharacterSize(static_cast<unsigned>(64 * diff));
            text.setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);

            std::string prefix = (i == selected ? "> " : "   ");
            text.setString(prefix + options[i]);

            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width / 2.f,
                           bounds.height / 2.f);

            text.setPosition(
                baseX,
                baseY + (static_cast<float>(i) - static_cast<float>(options.size() - 1) / 2.f) * stepY
            );

            this->window_.draw(text);
        }

        this->window_.display();
    }
    return -1;
}

bool Renderer::isOpen() const {
    return this->window_.isOpen();
}

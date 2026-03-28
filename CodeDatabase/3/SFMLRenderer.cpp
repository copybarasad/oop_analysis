#include "ViewController/Renderers/SFMLRenderer.hpp"

SFMLRenderer::SFMLRenderer(sf::RenderWindow& window) : window(window) {}

bool SFMLRenderer::isAlive() {
    return window.isOpen();
}

void SFMLRenderer::draw(GameDTO& dto) {
    if (!isAlive()) return;
    
    window.clear();
    if (dto.state == GameState::OnWalk || dto.state == GameState::OnCast) {
        drawGame(dto);
        drawUI(dto);
    }

    if (dto.state == GameState::Menu) {
        drawStartMenu(dto);
    }

    if (dto.state == GameState::Improve) {
        drawImproveMenu(dto);
    }

    if (dto.state == GameState::Win) {
        drawWinMenu(dto);
    }

    if (dto.state == GameState::Lose) {
        drawLoseMenu(dto);
    }

    window.display();
}

void SFMLRenderer::drawImproveMenu(GameDTO& dto) {
    int Y = CFG::GRID_STEP_Y * 0.5;

    sf::Font font;
    font.loadFromFile(Paths::FONT_PATH);

    drawText("Поздравляю, вы прошли уровень!", CFG::LARGE_FONT_SIZE, font, CFG::GRID_STEP_X * 1.2, Y);
    Y += CFG::GRID_STEP_Y * 1.5;


    drawText("Дальше будет сложнее, так что выберите себе одно из улучшений :", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X * 1.2, Y);
    Y += CFG::GRID_STEP_Y * 2;

    drawText("1 улучшение. Это даст ему +25 у урону и увеличит дальность применения.", CFG::MID_FONT_SIZE, font, CFG::GRID_STEP_X * 0.5, Y);
    Y += CFG::GRID_STEP_Y;

    drawText("2 улучшение. Это даст ему +50 у урону и увеличит дальность применения", CFG::MID_FONT_SIZE, font, CFG::GRID_STEP_X * 0.5, Y);
    Y += CFG::GRID_STEP_Y;

    drawText("3 улучшение. Это даст ему +100 у урону и увеличит дальность применения.", CFG::MID_FONT_SIZE, font, CFG::GRID_STEP_X * 0.5, Y);
    Y += CFG::GRID_STEP_Y;

    drawText("4 улучшение. Это даст вам +40 урона в ближнем и +20 урона в дальнем бою.", CFG::MID_FONT_SIZE, font, CFG::GRID_STEP_X * 0.5, Y);
    Y += CFG::GRID_STEP_Y;

    drawText("5 улучшение. Это даст вас +250 hp.", CFG::MID_FONT_SIZE, font, CFG::GRID_STEP_X * 0.5, Y);
    Y += CFG::GRID_STEP_Y;
}

void SFMLRenderer::drawGame(GameDTO& dto) {
    int mapX = dto.map.width;
    int mapY = dto.map.height;
    int wHeight = window.getSize().y;

    sf::Vector2f size;
    if (mapX > mapY) {
        size = sf::Vector2f((wHeight) / mapX, (wHeight) / mapX);
    } else {
        size = sf::Vector2f((wHeight) / mapY, (wHeight) / mapY);
    }
    
    
    for (auto& rawField : dto.map.FPs) {
        sf::Sprite sprite;

        switch (rawField.type)
        {
        case FieldType::PLAYER:
            loadPlayerTexture(sprite, dto.player);
            break;
        case FieldType::ENEMY:
            loadEnemyTexture(sprite, dto.enemies, rawField.coord);
            break;
        case FieldType::ATTACK_TOWER:
            loadAttackTowerTexture(sprite, dto.towers, rawField.coord);
            break;
        case FieldType::SPAWN_TOWER:
            sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_TOWER));
            break;
        case FieldType::FIELD_BLOCK:
            sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_BLOCK));
            break;
        case FieldType::FIELD_SLOW:
            sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_SLOW));
            break;
        case FieldType::FIELD_TRAP:
            sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_TRAP));
            break;
        default:
            sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_DEFAULT));
            break;
        }

        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        float scaleX = size.x / float(textureSize.x);
        float scaleY = size.y / float(textureSize.y);

        sprite.setScale(scaleX, scaleY);
        sprite.setPosition(size.x * (rawField.coord.x), size.y * (rawField.coord.y));

        window.draw(sprite);

        if (std::find(dto.shooted.begin(), dto.shooted.end(), rawField.coord) != dto.shooted.end()) {
            if (rawField.type == FieldType::FIELD_TRAP || rawField.type == FieldType::FIELD_DEFAULT || rawField.type == FieldType::FIELD_SLOW) {
                sf::Sprite sprite;
                sprite.setTexture(tm.getTexture(RenderFieldType::BULLET));

                sf::Vector2u textureSize = sprite.getTexture()->getSize();
                float scaleX = size.x / float(textureSize.x);
                float scaleY = size.y / float(textureSize.y);
                sprite.setScale(scaleX, scaleY);
                sprite.setPosition(size.x * (rawField.coord.x), size.y * (rawField.coord.y));
                sprite.setColor(sf::Color(255, 150, 150, 100));

                window.draw(sprite);
            }
        }
    }
    

    if (dto.state == GameState::OnCast) {
        sf::RectangleShape shape(size);
        shape.setFillColor(sf::Color(0, 0, 255, 120));
        for (auto& f : dto.cast.castRange) {
            shape.setPosition(size.x * (f.x), size.y * (f.y));
            window.draw(shape);
        }
        shape.setFillColor(sf::Color(0, 255, 255, 120));
        shape.setPosition(size.x * (dto.cast.target.x), size.y * (dto.cast.target.y));
        window.draw(shape);
    }
}

void SFMLRenderer::loadPlayerTexture(sf::Sprite& sprite, PlayerDTO player) {
    if (player.disabled) {
        sprite.setTexture(tm.getTexture(RenderFieldType::PLAYER_WAITING));
    } else if (player.rangeForm) {
        sprite.setTexture(tm.getTexture(RenderFieldType::PLAYER_RANGE));
    } else {
        sprite.setTexture(tm.getTexture(RenderFieldType::PLAYER_MELEE));
    }

    float filter = float(player.hp) / player.maxHP;
    sprite.setColor(sf::Color(255, 255 * filter, 255 * filter));
}

void SFMLRenderer::loadEnemyTexture(sf::Sprite& sprite, std::vector<EnemyDTO> enemies, Coordinates coord) {
    EnemyDTO enemy;
    for (auto& e : enemies) {
        if (e.coord == coord) {
            enemy = e;
            break;
        }
    }

    if (enemy.disabled) {
        sprite.setTexture(tm.getTexture(RenderFieldType::ENEMY_WAITING));
    } else {
        sprite.setTexture(tm.getTexture(RenderFieldType::ENEMY));
    }

    float filter = float(enemy.hp) / enemy.maxHP;
    sprite.setColor(sf::Color(255, 255 * filter, 255 * filter));
}

void SFMLRenderer::loadAttackTowerTexture(sf::Sprite& sprite, std::vector<TowerDTO> towers, Coordinates coord) {
    TowerDTO tower;

    for (auto& t : towers) {
        if (t.coord == coord) {
            tower = t;
            break;
        }
    }

    sprite.setTexture(tm.getTexture(RenderFieldType::FIELD_TOWER_ATTACK));
    
    float filter = float(tower.hp) / tower.maxHp;
    sprite.setColor(sf::Color(255, 255 * filter, 255 * filter));
}

void SFMLRenderer::drawUI(GameDTO& dto) {
    float pad = CFG::GAMEBAR_PADDING;
    int width = CFG::GAMEBAR_WIDTH;
    sf::RectangleShape background(sf::Vector2f(width, window.getSize().y));
    background.setPosition(CFG::GAMEBAR_START_X, 0);
    background.setFillColor(sf::Color(255, 255, 255, 50));
    window.draw(background);

    sf::Font font;
    font.loadFromFile(Paths::FONT_PATH);
    float Y = 0;
    float X = CFG::GAMEBAR_START_X;

    drawText("Запущен уровень: " + std::to_string(dto.stats.lvl), CFG::SMALL_FONT_SIZE, font, X, Y);
    Y += pad;
    drawText("Ваши очки: " + std::to_string(dto.stats.points) + "/" + std::to_string(dto.stats.endLevelPoints), CFG::SMALL_FONT_SIZE, font, X, Y);
    if (dto.stats.points >= dto.stats.endLevelPoints) {
        drawText("ВЫ ПРОШЛИ УРОВЕНЬ!!!", CFG::SMALL_FONT_SIZE, font, X + pad * 5, Y);
    }
    Y += pad;

    // Игрок
    drawText("ИГРОК:", CFG::BIG_FONT_SIZE, font, X + pad * 8.5, Y);
    Y += pad * 2;

    float percent = float(dto.player.hp) / dto.player.maxHP;
    sf::RectangleShape barBack(sf::Vector2f(width - pad * 8, pad));
    barBack.setFillColor(sf::Color(60, 60, 60));
    barBack.setPosition(X + pad * 6, Y + CFG::MID_FONT_SIZE / 4);
    window.draw(barBack);
    sf::RectangleShape bar(sf::Vector2f((width - pad * 8) * percent, pad));
    bar.setFillColor(sf::Color(200, 50, 50));
    bar.setPosition(X + pad * 6, Y + CFG::MID_FONT_SIZE / 4);
    window.draw(bar);

    drawText("HP: " + std::to_string(dto.player.hp) + "/" + std::to_string(dto.player.maxHP), CFG::MID_FONT_SIZE, font, X, Y);
    Y += pad * 1.5;
    drawText("Урон: " + std::to_string(dto.player.dmg), CFG::MID_FONT_SIZE, font, X, Y);
    drawText(dto.player.rangeForm ? "(дальний бой)" : "(ближний бой)", CFG::MID_FONT_SIZE, font, X + pad * 4, Y);
    Y += pad * 1.5;

    // Спеллы
    drawText("В вашей руке с заклинаниями:", CFG::MID_FONT_SIZE, font, X, Y);
    Y += pad;
    drawText("Максимум заклинаний: " + std::to_string(dto.cast.hand.capacity), CFG::SMALL_FONT_SIZE, font, X + CFG::GRID_STEP_X * 2.5, Y);
    Y += pad;
    Y -= pad;
    int spellInd = 1;
    for (auto s : dto.cast.hand.spells) {
        Y += pad;
        switch (s)
        {
        case SpellType::TargetSpell:
            drawText(std::to_string(spellInd) + " - Заклинание прямого урона. Урон = " + std::to_string(dto.cast.hand.targetLvl * 50 + 50), CFG::SMALL_FONT_SIZE, font, X, Y);
            break;
        case SpellType::AoESpell :
            drawText(std::to_string(spellInd) + " - Заклинание урона по области. Урон = " + std::to_string(dto.cast.hand.areaLvl * 25 + 25), CFG::SMALL_FONT_SIZE, font, X, Y);
            break;
        case SpellType::TrapSpell:
            drawText(std::to_string(spellInd) + " - Заклинание ловушки. Урон = " + std::to_string(dto.cast.hand.trapLvl * 25 + 50), CFG::SMALL_FONT_SIZE, font, X, Y);
            break;
        }
        spellInd++;
    }
    Y += pad * 1.5;

    // Враги
    drawText("ВРАГИ:", CFG::BIG_FONT_SIZE, font, X + pad * 8.5, Y);
    Y += pad * 1.5;

    float itemHeight = pad * 2;
    float itemWidth  = pad * 3;
    float gap = pad * 0.5;

    float startX = X + pad;
    float startY = Y;
    float maxWidth = CFG::GAMEBAR_WIDTH - pad;
    int itemsPerRow = int(maxWidth / (itemWidth + gap));

    int index = 0;
    float curX = startX;
    float curY = startY;
    
    for (auto& e : dto.enemies) {
        if (index % itemsPerRow == 0 && index != 0) {
            curX = startX;
            curY += itemHeight + gap;
        }
        
        // Карточка
        sf::RectangleShape card(sf::Vector2f(itemWidth, itemHeight));
        card.setPosition(curX, curY);
        card.setFillColor(sf::Color(255, 255, 255, 40));
        window.draw(card);

        // Текст
        drawText("Враг:", CFG::SMALL_FONT_SIZE, font, curX, curY);
        drawText("hp: " + std::to_string(e.hp) + "/" + std::to_string(e.maxHP), CFG::SMALL_FONT_SIZE, font, curX, curY + pad * 0.6);
        drawText("Урон: " + std::to_string(e.dmg), CFG::SMALL_FONT_SIZE, font, curX, curY + pad * 1.2);

        // Разделитель между карточками
        curX += itemWidth + gap;
        index++;
    }
    if (index > 0) Y = curY + pad * 3;
   

    curX = startX;
    curY = Y;
    index = 0;

    for (auto& t : dto.towers) {
        if (index % itemsPerRow == 0 && index != 0) {
            curX = startX;
            curY += itemHeight + gap;
        }

        sf::RectangleShape card(sf::Vector2f(itemWidth, itemHeight));
        card.setPosition(curX, curY);
        card.setFillColor(sf::Color(255, 255, 255, 40));
        window.draw(card);

        drawText("Башня:", CFG::SMALL_FONT_SIZE, font, curX, curY);
        if (t.isAttack) {
            drawText("hp: " + std::to_string(t.hp) + "/" + std::to_string(t.maxHp), CFG::SMALL_FONT_SIZE, font, curX, curY + pad * 0.6);
            drawText("КД: " + std::to_string(t.steps) + "/" + std::to_string(t.coolDown), CFG::SMALL_FONT_SIZE, font, curX, curY + pad * 1.2);
        } else {
            drawText("До врага: " + std::to_string(t.steps) + "/" + std::to_string(t.coolDown), CFG::SMALL_FONT_SIZE, font, curX, curY + pad * 0.6);
        }

        curX += itemWidth + gap;
        index++;
    }
    Y = curY + pad * 2;
}

void SFMLRenderer::drawText(std::string str, int size, sf::Font& font, int x, int y) {
    sf::Text text(sf::String::fromUtf8(str.begin(), str.end()), font, size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
    window.draw(text);
}

void SFMLRenderer::drawStartMenu(GameDTO& dto) {
    int Y = CFG::GRID_STEP_Y;

    sf::Font font;
    font.loadFromFile(Paths::FONT_PATH);
    drawText("Привет!", CFG::LARGE_FONT_SIZE, font, CFG::GRID_STEP_X * 4, Y);
    Y += CFG::GRID_STEP_Y * 2;
    drawText("Вы можете запустить новую игру", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
    Y += CFG::GRID_STEP_Y;
    drawText("Вы можете загрузить сохранение", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
}


void SFMLRenderer::drawWinMenu(GameDTO& dto) {
    int Y = CFG::GRID_STEP_Y;

    sf::Font font;
    font.loadFromFile(Paths::FONT_PATH);
    drawText("Вы выиграли!", CFG::LARGE_FONT_SIZE, font, CFG::GRID_STEP_X * 4, Y);
    Y += CFG::GRID_STEP_Y * 2;
    drawText("Вы можете запустить новую игру", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
    Y += CFG::GRID_STEP_Y;
    drawText("Вы можете загрузить сохранение", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
}

void SFMLRenderer::drawLoseMenu(GameDTO& dto) {
    int Y = CFG::GRID_STEP_Y;

    sf::Font font;
    font.loadFromFile(Paths::FONT_PATH);
    drawText("Вы проиграли!", CFG::LARGE_FONT_SIZE, font, CFG::GRID_STEP_X * 4, Y);
    Y += CFG::GRID_STEP_Y * 2;
    drawText("Вы можете запустить новую игру", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
    Y += CFG::GRID_STEP_Y;
    drawText("Вы можете загрузить сохранение", CFG::BIG_FONT_SIZE, font, CFG::GRID_STEP_X, Y);
}
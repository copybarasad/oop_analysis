#include "SFMLRenderer.h"
#include "Interfaces/Game.h"
#include <iostream>


SFMLRenderer::SFMLRenderer(Game* game, GameTheme theme) : game(game)
{
    this->width = game->getSize().X();
    this->height = game->getSize().Y();
    this->window = new sf::RenderWindow(sf::VideoMode({ width, height }), "Stranger Things 5 season.");
    this->window->setKeyRepeatEnabled(false);
    setTheme(theme);
}


SFMLRenderer::SFMLRenderer(SFMLRenderer&& other) noexcept
    : game(other.game)
    , window(other.window)
    , width(other.width)
    , height(other.height)
    , backgroundColor(other.backgroundColor)
    , playerColor(other.playerColor)
    , enemyColor(other.enemyColor)
    , enemyDeadColor(other.enemyDeadColor)
    , allyColor(other.allyColor)
    , trapColor(other.trapColor)
    , obstuctionCellsColor(other.obstuctionCellsColor)
    , slowCellsColor(other.slowCellsColor)
    , towerColor(other.towerColor)
    , towerDestroyedColor(other.towerDestroyedColor)
    , archerTowerColor(other.archerTowerColor)
{
    other.game = nullptr;
    other.window = nullptr;
    other.width = 0;
    other.height = 0;
}


sf::RenderWindow* SFMLRenderer::getWindow()
{
    return window;
}


void SFMLRenderer::resizeWindow(const unsigned int newWidth, const unsigned int newHeight, const unsigned int expansion)
{
    if (window->getSize().x == newWidth && window->getSize().y == newHeight)
        return;

    window->setSize({ newWidth, newHeight });
    
    sf::View view(sf::FloatRect({ 0., 0. }, { (float)newWidth, (float)newHeight }));
    window->setView(view);
    auto desktop = sf::VideoMode::getDesktopMode();
    window->setPosition({(int) (desktop.size.x / 2 - window->getSize().x / 2), (int) (desktop.size.y / 2 - window->getSize().y / 2 )});
}


void SFMLRenderer::renderMap()
{
    
    window->clear(sf::Color::White);
	renderField();
	renderPlayer();
    renderPlayInfo();
    window->display();
}


void SFMLRenderer::renderPlayInfo()
{
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderPlayInfo: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    const auto* tower = game->getExecuteUseCase()->getGameBoard()->getTower();
    const auto* player = game->getExecuteUseCase()->getGameBoard()->getPlayer();

    int count = -7;
    unsigned short fontSize = 18;
    unsigned short spaceBetween = 24;
    unsigned short padding = 30;


    sf::Text text(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::Black);
    text.setLetterSpacing(2);
    sf::Vector2u size = window->getSize();

    text.setString("Game level: " + std::to_string(game->getGameLevel()) + "/" + std::to_string(game->getGameLevelMax()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("Player health: " + std::to_string(player->getHealth()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("Player damage: " + std::to_string(player->getDamage()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("Player score: " + std::to_string(player->getScore()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(player->isLongFight() ? "Distance attack: Long" : "Distance attack: Short");
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(player->isFastMode() ? "Speed mode: Fast" : "Speed mode: Slow");
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("Player steps: " + std::to_string(player->getSteps()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("Tower health: " + std::to_string(tower->getHealth()));
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);


    //################ MARKET ######################
    text.setString("Spells market: "
        + std::string("AreaSpell ") + std::to_string(abs(game->getExecuteUseCase()->getPointSystem().getAreaSpellCost()))
        + std::string(", DirectSpell ") + std::to_string(abs(game->getExecuteUseCase()->getPointSystem().getDirectSpellCost()))
        + std::string(",\nTrapSpell ") + std::to_string(abs(game->getExecuteUseCase()->getPointSystem().getTrapSpellCost()))
        + std::string(", AllySpell ") + std::to_string(abs(game->getExecuteUseCase()->getPointSystem().getAllySpellCost()))
        + std::string(", UpgradeSpell ") + std::to_string(abs(game->getExecuteUseCase()->getPointSystem().getUpgradeSpellCost()))
    );
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);
    count++;
    //################ MARKET ######################


    const auto* hand = player->getHand();

    std::string spells = "";
    for (const auto* spell : hand->getSpells())
    {
        spells += spell->getName() + "( lvl " + std::to_string(spell->getLevel()) + ") ";
    }

    text.setString("Available: " + spells);
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setCharacterSize(fontSize);
    text.setString("Spell in the hand: " + hand->getSpellNameInTheHand());
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(hand->getSpellInfo());
    text.setPosition({ (float)game->fieldWidth() + padding, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);
}


void SFMLRenderer::renderMenu()
{
    //вынести resize в command.
    resizeWindow(game->getSize().X(), game->getSize().Y(), 0);
    //game->resizeGameWindow(*window, game->getSize(), true);
    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderMenu: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    auto size = game->getSize();
    int count = -3;
    unsigned short spaceBetween = 40;

    text.setString(L"ГЛАВНОЕ МЕНЮ");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.X() / 2, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"1 - Новая игра");
    text.setCharacterSize(24);
    text.setOrigin({ 0, 0 });
    text.setPosition({ (float)size.X() / 2 - text.getLocalBounds().getCenter().x, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    if (!game->hasActiveLevel()) {
        text.setFillColor(sf::Color(128, 128, 128));
    }
    text.setString(L"2 - Продолжить игру");
    text.setPosition({ (float)size.X() / 2 - text.getLocalBounds().getCenter().x, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setFillColor(sf::Color::Black);
    text.setString(L"3 - Загрузить игру");
    text.setPosition({ (float)size.X() / 2 - text.getLocalBounds().getCenter().x, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"4 - Справка");
    text.setPosition({ (float)size.X() / 2 - text.getLocalBounds().getCenter().x, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"Q - Выход");
    text.setPosition({ (float)size.X() / 2 - text.getLocalBounds().getCenter().x, (float)size.Y() / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderPause()
{
    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderPause: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    sf::Vector2u size = window->getSize();
    int count = -1;
    unsigned short spaceBetween = 40;

    text.setString(L"ПАУЗА");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"ESC - Продолжить, Q - меню");
    text.setCharacterSize(24);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"S - Сохранить игру, R - Загрузить игру");
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderGameWin()
{
    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderGameWin: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    sf::Vector2u size = window->getSize();
    int count = -1;
    unsigned short spaceBetween = 40;

    text.setString(L"ВЫ ПРОШЛИ ИГРУ!");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"Q - Меню");
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderGameOver()
{
    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderGameOver: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    sf::Vector2u size = window->getSize();
    int count = -1;
    unsigned short spaceBetween = 40;

    text.setString(L"УРОВЕНЬ " + std::to_wstring(game->getGameLevel()) + L" ПРОВАЛЕН!");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"R - Повторить");
    text.setCharacterSize(24);
    text.setOrigin({ 0, 0 });
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"Q - Меню");
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderLevelWin()
{

    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderLevelWin: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    sf::Vector2u size = window->getSize();
    int count = -1;
    unsigned short spaceBetween = 40;

    text.setString(L"УРОВЕНЬ " + std::to_wstring(game->getGameLevel()) + L" ПРОЙДЕН!");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"W - Следующий уровень");
    text.setCharacterSize(24);
    text.setOrigin({ 0, 0 });
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString(L"Q - Меню");
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderLevelBetween()
{
    window->clear(sf::Color::White);
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error(u8"Ошибка в renderLevelBetween: не удалось загрузить шрифт C:/Windows/Fonts/arial.ttf\n");
    }

    sf::Text text(font);

    sf::Vector2u size = window->getSize();
    int count = -1;
    unsigned short spaceBetween = 40;

    text.setString(L"ВЫБЕРИТЕ УЛУЧШЕНИЕ");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ (float)size.x / 2, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("1 - Player health: +" + std::to_string(game->getBuffHealth()));
    text.setCharacterSize(24);
    text.setOrigin({ 0, 0 });
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    text.setString("2 - Player damage: +" + std::to_string(game->getBuffDamage()));
    text.setPosition({ (float)size.x / 2 - text.getLocalBounds().getCenter().x, (float)size.y / 2 + spaceBetween * count++ });
    window->draw(text);

    window->display();
}


void SFMLRenderer::renderField() const
{
    //-----DRAW CELLS-----
    const auto* field = game->getExecuteUseCase()->getGameBoard()->getField();
    const auto& fieldCells = field->getField();
    for (int i = 0; i < field->getHeight(); ++i)
    {
        for (int j = 0; j < field->getWidth(); ++j)
        {
            sf::RectangleShape cellShape({ (float)field->getCellSize(), (float)field->getCellSize() });
            cellShape.setPosition({ (float)j * field->getCellSize(), (float)i * field->getCellSize() });
            if (!fieldCells[i][j].isObstruction())
                cellShape.setFillColor(obstuctionCellsColor);
            else if (fieldCells[i][j].isSlow())
                cellShape.setFillColor(slowCellsColor);
            else
                cellShape.setFillColor(backgroundColor);
            window->draw(cellShape);
        }
    }
    //-----DRAW CELLS-----


    //-----DRAW TRAPS-----
    for (auto& trap : game->getExecuteUseCase()->getGameBoard()->getTraps())
    {
        auto& point = trap.getPosition();
        sf::RectangleShape trapShape({ (float)field->getCellSize(), (float)field->getCellSize() });
        trapShape.setPosition({ (float)point.X() * field->getCellSize(), (float)point.Y() * field->getCellSize() });
        trapShape.setFillColor(trapColor);
        window->draw(trapShape);
    }
    //-----DRAW TRAPS-----


    //-----DRAW TOWER-----
    const auto* tower = game->getExecuteUseCase()->getGameBoard()->getTower();

    sf::RectangleShape towerShape({ (float)field->getCellSize(), (float)field->getCellSize() });

    Point p = tower->getPosition();

    towerShape.setPosition({ (float)p.X() * field->getCellSize(), (float)p.Y() * field->getCellSize() });
    if (tower->isDestroyed())
        towerShape.setFillColor(towerDestroyedColor);
    else
        towerShape.setFillColor(towerColor);

    window->draw(towerShape);
    //-----DRAW TOWER-----



    //-----DRAW ARCHERTOWER-----
    const auto& archerTower = game->getExecuteUseCase()->getGameBoard()->getArcherTower();
    sf::RectangleShape archerTowerShape({ (float)field->getCellSize(), (float)field->getCellSize() });

    p = archerTower->getPosition();

    archerTowerShape.setPosition({ (float)p.X() * field->getCellSize(), (float)p.Y() * field->getCellSize() });
    archerTowerShape.setFillColor(archerTowerColor);
    window->draw(archerTowerShape);

    //-----DRAW ARCHERTOWER-----


    //-----DRAW ENEMIES-----
    const auto& enemies = game->getExecuteUseCase()->getGameBoard()->getEnemies();
    for (int i = 0; i < enemies.size(); ++i)
    {   
        sf::CircleShape enemyShape((float)field->getCellSize() / 2);
        Point p = enemies[i].getPosition();
        enemyShape.setPosition({ (float)p.X() * field->getCellSize() , (float)p.Y() * field->getCellSize() });
        if (enemies[i].isAlive())
        {
            enemyShape.setFillColor(enemyColor);
        }
        else
        {
            enemyShape.setFillColor(enemyDeadColor);
        }
        window->draw(enemyShape);
    }

    const auto& allies = game->getExecuteUseCase()->getGameBoard()->getAllies();
    for (int i = 0; i < allies.size(); ++i)
    {
        sf::CircleShape allyShape((float)field->getCellSize() / 2);
        Point p = allies[i].getPosition();
        allyShape.setPosition({ (float)p.X() * field->getCellSize() , (float)p.Y() * field->getCellSize() });
        
        allyShape.setFillColor(allyColor);

        window->draw(allyShape);
    }
    //-----DRAW ENEMIES-----
}


void SFMLRenderer::renderPlayer() const
{
    const auto* field = game->getExecuteUseCase()->getGameBoard()->getField();
    const auto* player = game->getExecuteUseCase()->getGameBoard()->getPlayer();
    sf::CircleShape playerShape((float)field->getCellSize() / 2);

    playerShape.setPosition({ (float)player->getPosition().X() * field->getCellSize(), (float)player->getPosition().Y() * field->getCellSize() });

    playerShape.setFillColor(playerColor);

    window->draw(playerShape);
}


void SFMLRenderer::setTheme(GameTheme theme)
{
    switch (theme)
    {
    case GameTheme::DEFAULT:
        backgroundColor = sf::Color(4, 33, 12);
        playerColor = sf::Color(100, 250, 50);
        enemyColor = sf::Color(240, 17, 34);
        enemyDeadColor = sf::Color(107, 9, 15);
        allyColor = sf::Color(7, 182, 217);
        trapColor = sf::Color(99, 107, 15);
        obstuctionCellsColor = sf::Color(128, 128, 128);
        slowCellsColor = sf::Color(5, 82, 41);
        towerColor = sf::Color(230, 119, 9);
        towerDestroyedColor = sf::Color(77, 2, 20);
        archerTowerColor = sf::Color(252, 3, 90);
        break;
    case GameTheme::WINTER:
        backgroundColor = sf::Color::White;
        playerColor = sf::Color(250, 129, 7);
        enemyColor = sf::Color(240, 17, 34);
        enemyDeadColor = sf::Color(107, 9, 15);
        allyColor = sf::Color(7, 182, 217);
        trapColor = sf::Color(0, 0, 0);
        obstuctionCellsColor = sf::Color(180, 200, 210);
        slowCellsColor = sf::Color(100, 180, 255);
        towerColor = sf::Color(110, 181, 24);
        towerDestroyedColor = sf::Color(25, 41, 5);
        archerTowerColor = sf::Color(252, 3, 90);
        break;
    case GameTheme::DISERT:
        backgroundColor = sf::Color(255, 248, 158);
        playerColor = sf::Color(204, 135, 100);
        enemyColor = sf::Color(199, 80, 20);
        enemyDeadColor = sf::Color(107, 40, 4);
        allyColor = sf::Color(7, 182, 217);
        trapColor = sf::Color(0, 0, 0);
        obstuctionCellsColor = sf::Color(86, 58, 44);
        slowCellsColor = sf::Color(125, 108, 32);
        towerColor = sf::Color(110, 181, 24);
        towerDestroyedColor = sf::Color(25, 41, 5);
        archerTowerColor = sf::Color(252, 3, 90);
        break;
    }
}


SFMLRenderer::~SFMLRenderer()
{
    if (window != nullptr)
    {
        delete window;
        window = nullptr;
    }
}
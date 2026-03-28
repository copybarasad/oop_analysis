#include "../gui_headers/gameGUI.hpp"

GameGUI::GameGUI() : window(sf::VideoMode(25*40+350, 20*40), "Game") {
    loadTextures();
    setupUI();
}

void GameGUI::loadTextures(){
    try{
        textureManager.loadTexture("empty", "resources/empty_cell.jpeg");
        textureManager.loadTexture("obstacle", "resources/obstacle_cell.jpeg");
        textureManager.loadTexture("slow", "resources/slow_cell.jpeg");
        textureManager.loadTexture("trap", "resources/trap_cell.jpeg");
        textureManager.loadTexture("player", "resources/player.png");
        textureManager.loadTexture("enemy", "resources/enemy.png");
        textureManager.loadTexture("ally", "resources/ally.png");
        textureManager.loadTexture("enemyBuilding", "resources/enemy_building.jpeg");
        textureManager.loadTexture("enemyTower", "resources/enemy_tower.jpeg");
    }
    catch (const std::exception& e){
        std::cerr << "Error loading textures: " << e.what() << std::endl;
    }
}

void GameGUI::setupUI(){
    if (!font.loadFromFile("resources/font.ttf"))
        std::cerr << "Error loading font" << std::endl;
    
    pointsText.setFont(font);
    pointsText.setCharacterSize(20);
    pointsText.setFillColor(sf::Color::White);
    
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::Red);
    
    manaText.setFont(font);
    manaText.setCharacterSize(20);
    manaText.setFillColor(sf::Color::Blue);
    
    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::Yellow);
    
    messageText.setFont(font);
    messageText.setCharacterSize(16);
    messageText.setFillColor(sf::Color::White);
    
    enemyInfoWindow.setup();
}

void GameGUI::render(const GameState& state, const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings){
    switch (state.state){
        case GameState::MAIN_MENU:
            renderMainMenu(keyBindings);
            break;
        case GameState::PLAYING:
            renderPlaying(state, gameManager, currentLevel, keyBindings);
            break;
        case GameState::LEVEL_COMPLETE:
            renderLevelComplete(keyBindings);
            break;
        case GameState::GAME_OVER:
            renderGameOver(keyBindings);
            break;
        case GameState::UPGRADING:
            renderUpgrading(keyBindings);
            break;
        case GameState::BUYING_SPELL:
            renderBuyingSpell(keyBindings);
            break;
    }
}

void GameGUI::renderMainMenu(const KeyBindingsInfo& keyBindings){
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(30, 30, 50));
    window.draw(background);
    
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(48);
    title.setString("GAME");
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(window.getSize().x/2-120, 100);
    window.draw(title);
    
    std::vector<std::string> options ={
        keyBindings.mainMenuBindings.at("NEW_GAME")+". New Game",
        keyBindings.mainMenuBindings.at("CONTINUE_GAME")+". Continue Game", 
        keyBindings.mainMenuBindings.at("EXIT")+". Exit"
    };
    renderMenuOptions(options, 250, keyBindings);
}

void GameGUI::renderPlaying(const GameState& state, const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings){
    if (!gameManager)
        return;
    
    renderField(gameManager);
    renderSidebar(gameManager, currentLevel, keyBindings);
    
    if (state.showingAttackRadius){
        renderAttackRadius(gameManager);
        
        messageText.setString("Attack mode: Click to attack");
        messageText.setPosition(10, 10);
        messageText.setFillColor(sf::Color::Red);
        window.draw(messageText);
    }
    
    if (state.castingSpell){
        renderSpellRadius(gameManager);
        
        messageText.setString("Spell casting: Select target");
        messageText.setPosition(10, 10);
        messageText.setFillColor(sf::Color::Magenta);
        window.draw(messageText);
    }
    
    if (state.showingEnemyInfo && state.selectedEnemy)
        renderEnemyInfo(state);
}

void GameGUI::renderField(const GameManager* gameManager){
    Field field = gameManager->getField();
    
    for (int y=0; y < field.getHeight(); y++){
        for (int x=0; x < field.getWidth(); x++){
            sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
            cellShape.setPosition(x*CELL_SIZE, y*CELL_SIZE);
            
            Cell* cell = field.getCell({x, y});
            switch (cell->getType()){
                case cellType::EMPTY:
                    if (textureManager.hasTexture("empty"))
                        cellShape.setTexture(&textureManager.getTexture("empty"));
                    else
                        cellShape.setFillColor(sf::Color::White);
                    break;
                case cellType::OBSTACLE:
                    if (textureManager.hasTexture("obstacle"))
                        cellShape.setTexture(&textureManager.getTexture("obstacle"));
                    else
                        cellShape.setFillColor(sf::Color::Black);
                    break;
                case cellType::SLOW:
                    if (textureManager.hasTexture("slow"))
                        cellShape.setTexture(&textureManager.getTexture("slow"));
                    else
                        cellShape.setFillColor(sf::Color::Blue);
                    break;
                case cellType::TRAP:
                    if (textureManager.hasTexture("trap"))
                        cellShape.setTexture(&textureManager.getTexture("trap"));
                    else
                        cellShape.setFillColor(sf::Color(255, 165, 0));
                    break;
            }
            
            window.draw(cellShape);
        }
    }
    
    EnemyManager enemyManager = gameManager->getEnemyManager();
    for (Enemy* enemy : enemyManager.getEnemies()){
        std::pair <int, int> enemyPos = enemy->getCoordinates();
        sf::RectangleShape enemyShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
        enemyShape.setPosition(enemyPos.first*CELL_SIZE, enemyPos.second*CELL_SIZE);

        if (textureManager.hasTexture("enemy"))
            enemyShape.setTexture(&textureManager.getTexture("enemy"));
        else
            enemyShape.setFillColor(sf::Color::Red);
        window.draw(enemyShape);
    }
    
    AllyManager allyManager = gameManager->getAllyManager();
    for (Ally* ally : allyManager.getAllies()){
        std::pair <int, int> allyPos = ally->getCoordinates();
        sf::RectangleShape allyShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
        allyShape.setPosition(allyPos.first*CELL_SIZE, allyPos.second*CELL_SIZE);

        if (textureManager.hasTexture("ally"))
            allyShape.setTexture(&textureManager.getTexture("ally"));
        else
            allyShape.setFillColor(sf::Color::Green);
        window.draw(allyShape);
    }
    
    Player player = gameManager->getPlayer();
    std::pair <int, int> playerPos = player.getCoordinates();
    sf::RectangleShape playerShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
    playerShape.setPosition(playerPos.first*CELL_SIZE, playerPos.second*CELL_SIZE);

    if (textureManager.hasTexture("player"))
        playerShape.setTexture(&textureManager.getTexture("player"));
    else
        playerShape.setFillColor(sf::Color::Cyan);
    window.draw(playerShape);
    
    EnemyBuilding enemyBuilding = gameManager->getEnemyBuilding();
    std::pair <int, int> buildingPos = enemyBuilding.getCoordinates();
    sf::RectangleShape buildingShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
    buildingShape.setPosition(buildingPos.first*CELL_SIZE, buildingPos.second*CELL_SIZE);

    if (textureManager.hasTexture("enemyBuilding"))
        buildingShape.setTexture(&textureManager.getTexture("enemyBuilding"));
    else
        buildingShape.setFillColor(sf::Color::Magenta);
    window.draw(buildingShape);
    
    EnemyTower enemyTower = gameManager->getEnemyTower();
    std::pair <int, int> towerPos = enemyTower.getCoordinates();
    sf::RectangleShape towerShape(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
    towerShape.setPosition(towerPos.first*CELL_SIZE, towerPos.second*CELL_SIZE);
    
    if (textureManager.hasTexture("enemyTower"))
        towerShape.setTexture(&textureManager.getTexture("enemyTower"));
    else
        towerShape.setFillColor(sf::Color(128, 0, 128));
    window.draw(towerShape);
}

void GameGUI::renderSidebar(const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings){
    if (!gameManager)
        return;
    
    Player player = gameManager->getPlayer();
    
    sf::RectangleShape sidebar(sf::Vector2f(SIDEBAR_WIDTH, window.getSize().y));
    sidebar.setPosition(window.getSize().x-SIDEBAR_WIDTH, 0);
    sidebar.setFillColor(sf::Color(50, 50, 50, 230));
    window.draw(sidebar);
    
    sf::Text sidebarTitle;
    sidebarTitle.setFont(font);
    sidebarTitle.setCharacterSize(20);
    sidebarTitle.setString("GAME INFO");
    sidebarTitle.setFillColor(sf::Color::Yellow);
    sidebarTitle.setPosition(window.getSize().x-SIDEBAR_WIDTH+10, 15);
    window.draw(sidebarTitle);
    
    float currentY = 50.0f;
    
    std::string levelStr = "Level: "+std::to_string(currentLevel);
    levelText.setCharacterSize(16);
    levelText.setString(levelStr);
    levelText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    window.draw(levelText);
    currentY += 30.0f;
    
    std::string healthStr = "HP: "+std::to_string(player.getHealth())+"/"+std::to_string(player.getMaxHealth());
    healthText.setCharacterSize(16);
    healthText.setString(healthStr);
    healthText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    window.draw(healthText);
    currentY += 30.0f;
    
    std::string manaStr = "Mana: "+std::to_string(player.getMana());
    manaText.setCharacterSize(16);
    manaText.setString(manaStr);
    manaText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    window.draw(manaText);
    currentY += 30.0f;
    
    std::string pointsStr = "Points: "+std::to_string(player.getPoints());
    pointsText.setCharacterSize(16);
    pointsText.setString(pointsStr);
    pointsText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    window.draw(pointsText);
    currentY += 30.0f;
    
    std::string fightTypeStr = "Fight: "+std::string(player.getFight() == fightType::NEAR ? "Near" : "Far");
    sf::Text fightText;
    fightText.setFont(font);
    fightText.setCharacterSize(16);
    fightText.setString(fightTypeStr);
    fightText.setFillColor(sf::Color::Cyan);
    fightText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    window.draw(fightText);
    currentY += 35.0f;
    
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, currentY);
    std::string controlsStr = "CONTROLS:\n";
    controlsStr += keyBindings.playingBindings.at("MOVE_UP")+"-Up ";
    controlsStr += keyBindings.playingBindings.at("MOVE_DOWN")+"-Down ";
    controlsStr += keyBindings.playingBindings.at("MOVE_LEFT")+"-Left ";
    controlsStr += keyBindings.playingBindings.at("MOVE_RIGHT")+"-Right\n";
    controlsStr += keyBindings.playingBindings.at("TOGGLE_FIGHT")+"-Toggle Fight\n";
    controlsStr += keyBindings.playingBindings.at("SHOW_ATTACK_RADIUS")+"-Attack Mode\n";
    controlsStr += keyBindings.playingBindings.at("BUY_SPELL")+"-Buy Spell\n";
    controlsStr += keyBindings.playingBindings.at("SAVE_GAME")+"-Save Game\n";
    controlsStr += keyBindings.playingBindings.at("CANCEL")+"-Cancel";
    
    controlsText.setString(controlsStr);
    window.draw(controlsText);
    currentY += 160.0f;
        
    renderSpells(gameManager, currentY, keyBindings);
}

void GameGUI::renderSpells(const GameManager* gameManager, float startY, const KeyBindingsInfo& keyBindings){
    if (!gameManager)
        return;
    
    Player player = gameManager->getPlayer();
    PlayerHand hand = player.getHand();
    const auto& spells = hand.getSpells();
    
    sf::Text spellsHeader;
    spellsHeader.setFont(font);
    spellsHeader.setCharacterSize(18);
    spellsHeader.setFillColor(sf::Color::Yellow);
    spellsHeader.setString("SPELLS ("+std::to_string(spells.size())+"/"+std::to_string(hand.getMaxSize())+")");
    spellsHeader.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, startY);
    window.draw(spellsHeader);
    
    startY += 35.0f;
    
    const float cardWidth = SIDEBAR_WIDTH-25;
    const float cardHeight = 50.0f;
    const float cardSpacing = 8.0f;
    
    float yPos = startY;
    
    for (size_t i = 0; i < spells.size(); ++i){
        if (yPos+cardHeight > window.getSize().y-30){
            sf::Text moreText;
            moreText.setFont(font);
            moreText.setCharacterSize(14);
            moreText.setFillColor(sf::Color(150, 150, 150));
            moreText.setString("... and "+std::to_string(spells.size()-i)+" more");
            moreText.setPosition(window.getSize().x-SIDEBAR_WIDTH+15, yPos);
            window.draw(moreText);
            break;
        }
        
        sf::RectangleShape cardBackground(sf::Vector2f(cardWidth, cardHeight));
        cardBackground.setPosition(window.getSize().x-SIDEBAR_WIDTH+12, yPos);
        cardBackground.setFillColor(sf::Color(70, 70, 70, 220));
        cardBackground.setOutlineThickness(2);
        
        if (dynamic_cast<AreaDamageSpell*>(spells[i]))
            cardBackground.setOutlineColor(sf::Color::Red);
        else if (dynamic_cast<DirectDamageSpell*>(spells[i]))
            cardBackground.setOutlineColor(sf::Color::Blue);
        else if (dynamic_cast<TrapSpell*>(spells[i]))
            cardBackground.setOutlineColor(sf::Color::Green);
        else if (dynamic_cast<AllySpell*>(spells[i]))
            cardBackground.setOutlineColor(sf::Color::Yellow);
        else if (dynamic_cast<ImproveSpell*>(spells[i]))
            cardBackground.setOutlineColor(sf::Color::White);
        else
            cardBackground.setOutlineColor(sf::Color::Magenta);
        
        window.draw(cardBackground);
        
        float textX = window.getSize().x-SIDEBAR_WIDTH+20;
        
        sf::Text spellName;
        spellName.setFont(font);
        spellName.setCharacterSize(14);
        spellName.setFillColor(sf::Color::White);
        spellName.setString(spells[i]->getName());
        spellName.setPosition(textX, yPos+5);
        window.draw(spellName);
        
        sf::Text spellStats;
        spellStats.setFont(font);
        spellStats.setCharacterSize(12);
        spellStats.setFillColor(sf::Color::Yellow);
        
        std::string stats = spells[i]->getInfo();
        if (stats.length() > 25){
            stats = stats.substr(0, 22)+"...";
        }
        spellStats.setString(stats);
        spellStats.setPosition(textX, yPos+25);
        window.draw(spellStats);
        
        sf::Text usageText;
        usageText.setFont(font);
        usageText.setCharacterSize(12);
        usageText.setFillColor(sf::Color::Magenta);
        usageText.setString("Key: "+std::to_string(i+1));
        usageText.setPosition(textX+cardWidth-45, yPos+30);
        window.draw(usageText);
        
        yPos += cardHeight+cardSpacing;
    }
    
    if (spells.empty()){
        sf::Text noSpellsText;
        noSpellsText.setFont(font);
        noSpellsText.setCharacterSize(16);
        noSpellsText.setFillColor(sf::Color(150, 150, 150));
        noSpellsText.setString("No spells\nPress"+keyBindings.playingBindings.at("BUY_SPELL")+" to buy");
        noSpellsText.setPosition(window.getSize().x-SIDEBAR_WIDTH+40, startY+50);
        window.draw(noSpellsText);
    }
}

void GameGUI::renderAttackRadius(const GameManager* gameManager){
    if (!gameManager)
        return;
    
    Field field = gameManager->getField();
    Player player = gameManager->getPlayer();

    int radius = std::min(field.getHeight(), field.getWidth());
    if (player.getFight() == fightType::NEAR)
        radius /= 10;
    else
        radius /= 5;
    
    std::pair <int, int> playerPos = player.getCoordinates();
    int centerX = playerPos.first, centerY = playerPos.second;
    
    for (int y = std::max(0, centerY-radius); y <= std::min(field.getHeight()-1, centerY+radius); y++){
        for (int x = std::max(0, centerX-radius); x <= std::min(field.getWidth()-1, centerX+radius); x++){
            if ((x-centerX)*(x-centerX)+(y-centerY)*(y-centerY) <= radius*radius){
                sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
                highlight.setPosition(x*CELL_SIZE, y*CELL_SIZE);
                highlight.setFillColor(sf::Color(255, 0, 0, 100));
                window.draw(highlight);
            }
        }
    }
}

void GameGUI::renderSpellRadius(const GameManager* gameManager){
    Player player = gameManager->getPlayer();
    std::pair <int, int> playerPos = player.getCoordinates();
    
    for (int y=0; y < gameManager->getField().getHeight(); y++){
        for (int x=0; x < gameManager->getField().getWidth(); x++){
            int dist = std::sqrt(std::pow(playerPos.first-x, 2)+std::pow(playerPos.second-y, 2));
            
            if (dist <= 5+gameManager->getCurrentLevel()){
                sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
                highlight.setPosition(x*CELL_SIZE, y*CELL_SIZE);
                highlight.setFillColor(sf::Color(255, 0, 255, 100));
                window.draw(highlight);
            }
        }
    }
}

void GameGUI::renderEnemyInfo(const GameState& state){
    if (state.selectedEnemy){
        std::pair <int, int> enemyPos = state.selectedEnemy->getCoordinates();
        sf::Vector2f screenPos(enemyPos.first*CELL_SIZE, enemyPos.second*CELL_SIZE);
        
        enemyInfoWindow.show(*state.selectedEnemy, screenPos);
        enemyInfoWindow.draw(window);
    }
}

void GameGUI::renderLevelComplete(const KeyBindingsInfo& keyBindings){
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setString("LEVEL COMPLETE!");
    title.setFillColor(sf::Color::Green);
    title.setPosition(window.getSize().x/2-150, 100);
    window.draw(title);
    
    std::vector<std::string> options ={
        keyBindings.levelCompleteBindings.at("CONTINUE_NEXT_LEVEL")+". Continue to next level",
        keyBindings.levelCompleteBindings.at("MAIN_MENU")+". Main menu"
    };
    renderMenuOptions(options, 200, keyBindings);
}

void GameGUI::renderGameOver(const KeyBindingsInfo& keyBindings){
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setString("GAME OVER");
    title.setFillColor(sf::Color::Red);
    title.setPosition(window.getSize().x/2-100, 100);
    window.draw(title);
    
    std::vector<std::string> options ={
        keyBindings.gameOverBindings.at("MAIN_MENU")+". Main Menu",
        keyBindings.gameOverBindings.at("EXIT")+". Exit"
    };
    renderMenuOptions(options, 200, keyBindings);
}

void GameGUI::renderUpgrading(const KeyBindingsInfo& keyBindings){
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(30);
    title.setString("CHOOSE UPGRADE");
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(window.getSize().x/2-120, 100);
    window.draw(title);
    
    std::vector<std::string> options ={
        keyBindings.upgradingBindings.at("UPGRADE_1")+". +20 Health",
        keyBindings.upgradingBindings.at("UPGRADE_2")+". +5 Damage",
        keyBindings.upgradingBindings.at("UPGRADE_3")+". +30 Mana", 
        keyBindings.upgradingBindings.at("UPGRADE_4")+". Upgrade Random Spell"
    };
    renderMenuOptions(options, 200, keyBindings);
}

void GameGUI::renderBuyingSpell(const KeyBindingsInfo& keyBindings){
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(30);
    title.setString("BUY SPELL (40 mana)");
    title.setFillColor(sf::Color::Cyan);
    title.setPosition(window.getSize().x/2-120, 100);
    window.draw(title);
    
    std::vector<std::string> options ={
        keyBindings.buyingSpellBindings.at("BUY_SPELL_1")+". Area Damage",
        keyBindings.buyingSpellBindings.at("BUY_SPELL_2")+". Direct Damage", 
        keyBindings.buyingSpellBindings.at("BUY_SPELL_3")+". Trap",
        keyBindings.buyingSpellBindings.at("BUY_SPELL_4")+". Ally",
        keyBindings.buyingSpellBindings.at("BUY_SPELL_5")+". Improve",
        keyBindings.buyingSpellBindings.at("CANCEL")+"-Cancel"
    };
    renderMenuOptions(options, 200, keyBindings);
}

void GameGUI::renderMenuOptions(const std::vector <std::string>& options, float startY, const KeyBindingsInfo& keyBindings){
    for (size_t i = 0; i < options.size(); ++i){
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setCharacterSize(24);
        optionText.setString(options[i]);
        optionText.setPosition(window.getSize().x/2-150, startY+i*40);
        window.draw(optionText);
    }
}

sf::RenderWindow& GameGUI::getWindow(){
    return window;
}
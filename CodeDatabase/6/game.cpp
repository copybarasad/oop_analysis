#include "../headers/game.hpp"

Game::Game() 
    : currentState(),
      currentLevel(1),
      gameRunning(true),
      renderer(){
    
    gameManager = std::make_unique <GameManager>();
    currentState.state = GameState::MAIN_MENU;
}

void Game::update(){
    if (currentState.state == GameState::PLAYING && gameManager){
        endType end = gameManager->checkGameEnd();
        if (end == endType::WIN)
            currentState.state = GameState::LEVEL_COMPLETE;
        else if (end == endType::LOSE)
            currentState.state = GameState::GAME_OVER;
    }
}

void Game::render(){
    renderer.render(currentState, gameManager.get(), currentLevel, currentKeyBindings);
}

void Game::executeGameTurn(){
    if (!gameManager)
        return;
    
    gameManager->allyAction();

    gameManager->enemyAction();
    
    EnemyBuilding enemyBuilding = gameManager->getEnemyBuilding();
    if ((gameManager->getMoveCount()-enemyBuilding.getStart())%enemyBuilding.getBreak() == 0)
        gameManager->spawnEnemy(10+currentLevel*3, 50+currentLevel*10);

    gameManager->towerAction();
    
    endType end = gameManager->checkGameEnd();
    if (end == endType::WIN)
        currentState.state = GameState::LEVEL_COMPLETE;
    else if (end == endType::LOSE)
        currentState.state = GameState::GAME_OVER;
}

void Game::handleMouseClick(const sf::Vector2i& position){
    if (currentState.state != GameState::PLAYING || !gameManager)
        return;
    
    int gridX = position.x/40;
    int gridY = position.y/40;
    std::pair <int, int> gridPos = {gridX, gridY};
    
    if (currentState.showingAttackRadius){
        gameManager->playerAttack();
        currentState.showingAttackRadius = false;
        executeGameTurn();
    }
    else if (currentState.castingSpell && currentState.showingSpellRadius){
        int spellIndex = currentState.selectedSpellIndex;
        bool success = gameManager->castSpell(spellIndex, gridPos);
        if (success){
            currentState.castingSpell = false;
            currentState.showingSpellRadius = false;
            currentState.selectedSpellIndex = -1;
            executeGameTurn();
        }
    }
    else{
        Enemy* enemy = gameManager->getEnemyManager().getEnemyAtCoordinates(gridPos);
        if (enemy){
            currentState.selectedEnemy = enemy;
            currentState.showingEnemyInfo = true;
        }
        else
            currentState.showingEnemyInfo = false;
    }
}

void Game::startNewGame(){
    try{
        gameManager = std::make_unique<GameManager>();
        currentLevel = 1;
    }
    catch (const std::exception& e){
        std::cout << "Game buil error: " << e.what() << std::endl;
    }
}

void Game::startNewLevel(){
    if (!gameManager)
        return;

    currentLevel++;
    gameManager->nextLevel(currentLevel);
}

void Game::loadGame(){
    SaveData data;
    if (saveManager.loadGame(data)){
        try{
            if (gameManager->getHash(data) != data.hash){
                std::cout << "Load error: incorrect load file" << std::endl;

                startNewGame();
                currentState.state = GameState::PLAYING;
                return;
            }
            gameManager->loadFromSaveData(data);

            currentLevel = data.level;
            gameManager->setCurrentLevel(currentLevel);
            currentState.state = static_cast<GameState::State>(data.savedState);
        }
        catch (const std::exception& e){
            std::cout << "Load error: " << e.what() << std::endl;

            startNewGame();
            currentState.state = GameState::PLAYING;
        }
    }
    else{
        startNewGame();
        currentState.state = GameState::PLAYING;
    }
}

void Game::saveGameCommand(){
    if (!gameManager)
        return;
    
    try{
        SaveData data = gameManager->getSaveData();
        data.savedState = static_cast<int>(currentState.state);
        data.hash = gameManager->getHash(data);

        saveManager.saveGame(data);
    }
    catch (const SaveException& e){
        std::cout << "Save error: " << e.what() << std::endl;
    }
}

void Game::movePlayer(direction dir){
    if (!gameManager || currentState.state != GameState::PLAYING)
        return;
    
    bool checkAction = gameManager->movePlayer(dir);
    if (checkAction)
        executeGameTurn();
}

void Game::toggleFight(){
    if (!gameManager || currentState.state != GameState::PLAYING)
        return;
    
    gameManager->changeFight();
    executeGameTurn();
}

void Game::showAttackRadius(){
    if (currentState.state == GameState::PLAYING)
        currentState.showingAttackRadius = true;
}

void Game::enterBuySpellMode(){
    if (currentState.state == GameState::PLAYING)
        currentState.state = GameState::BUYING_SPELL;
}

void Game::castSpell(int spellIndex){
    if (!gameManager || currentState.state != GameState::PLAYING)
        return;
    
    if (currentState.castingSpell){
        currentState.castingSpell = false;
        currentState.selectedSpellIndex = -1;
    }
    else{
        PlayerHand hand = gameManager->getPlayer().getHand();

        if (spellIndex < hand.getSize()){
            currentState.castingSpell = true;
            currentState.selectedSpellIndex = spellIndex;
            currentState.showingSpellRadius = true;
        }
    }
}

void Game::cancelAction(){
    if (currentState.state == GameState::BUYING_SPELL)
        currentState.state = GameState::PLAYING;
    else{
        currentState.showingAttackRadius = false;
        currentState.castingSpell = false;
        currentState.showingSpellRadius = false;
        currentState.selectedSpellIndex = -1;
    }
}

void Game::startNewGameCommand(){
    if (currentState.state == GameState::MAIN_MENU){
        startNewGame();
        currentState.state = GameState::PLAYING;
    }
}

void Game::continueGame(){
    if (currentState.state == GameState::MAIN_MENU)
        loadGame();
}

void Game::exitGame(){
    if (currentState.state == GameState::MAIN_MENU || currentState.state == GameState::GAME_OVER)
        gameRunning = false;
}

void Game::continueToNextLevel(){
    if (currentState.state == GameState::LEVEL_COMPLETE)
        currentState.state = GameState::UPGRADING;
}

void Game::returnToMainMenu(){
    if (currentState.state == GameState::LEVEL_COMPLETE || currentState.state == GameState::GAME_OVER)
        currentState.state = GameState::MAIN_MENU;
}

void Game::applyUpgrade(int upgradeIndex){
    if (!gameManager)
        return;
    
    if (gameManager->applyUpgrade(static_cast<UpgradeType>(upgradeIndex))){
        startNewLevel();
        currentState.state = GameState::PLAYING;
    }
}

void Game::buySpell(int spell){
    if (!gameManager)
        return;
    
    if (gameManager->addSpell((spellType)spell))
        currentState.state = GameState::PLAYING;

    executeGameTurn();
}

sf::RenderWindow& Game::getWindow(){
    return renderer.getRenderer()->getWindow();
}

bool Game::isRunning() const{
    return gameRunning;
}

const GameState& Game::getCurrentState() const{
    return currentState;
}

const KeyBindingsInfo& Game::getKeyBindingsInfo() const{
    return currentKeyBindings;
}

void Game::updateKeyBindingsInfo(KeyBindingsInfo currentKeyBindings){
    this->currentKeyBindings = currentKeyBindings;
}
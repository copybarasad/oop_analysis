#include "game.h"

Game::Game() 
    : map(10, 10),           
      entities(nullptr),      
      gameRunning(false),
      level(1),
      width(10),
      height(10),
      levelCompleted(false),
      playerDead(false),
      controller(new GameController<ConsoleInputHandler>(this)),
      visualizer() {  
    srand(time(0));
    entities = new EntityManager();
    
   visualizer.setInputHandler(controller->getInputHandlerPtr());
}

Game::~Game() {
    delete entities;
    delete controller;
}

void Game::runGameLoop() {
    visualizer.drawMainMenu();

    while (gameRunning) {
        visualizer.drawGame(map, *entities, level);

        bool playerActed = controller->processGameInput();
        if (!playerActed) continue; 

        entities->removeDeadTower(map);
        entities->removeDeadEnemies(map);
        entities->removeDeadAllies(map);
        entities->updateAllies(map);
        entities->updateEnemies(map);
        entities->updateBuildings(map);
        entities->updateTower(map);

        if (!entities->getPlayer().isAlive()) {
            playerDead = true;
            gameRunning = false;
        }

        if (entities->areAllEnemiesDead()) {
            levelCompleted = true;
        }

        if (levelCompleted) {
            visualizer.showMessage("Победа! Все враги уничтожены!");
            nextLevel();
        }

        if (playerDead) {
            visualizer.deadPlayer();
            GameCommand cmd = controller->getMenuCommand();
            switch (cmd.getType()) {
                case GameCommand::Type::MENU_SELECT_1: 
                    startNewGame(); 
                    break;
                default:
                    gameRunning = false; 
                    break;
            }
        }

    }
}

void Game::playerMove(int dx, int dy) {
    entities->movePlayer(dx, dy, map);
}

void Game::playerToggleCombat() {
    entities->getPlayer().switchCombatMode();
}

Position Game::selectTargetForSpell(int range) {
    Position cursor = entities->getPlayer().getPosition();
    Position originalCursor = cursor;
    while (gameRunning) {

        visualizer.drawTarget(map, *entities, cursor, range);


        GameCommand cmd = controller->getTargetCommand();

        Position newCursor = cursor;

        switch (cmd.getType()) {
            case GameCommand::Type::MOVE_UP:    newCursor = Position(cursor.getX(), cursor.getY() - 1); break;
            case GameCommand::Type::MOVE_DOWN:  newCursor = Position(cursor.getX(), cursor.getY() + 1); break;
            case GameCommand::Type::MOVE_LEFT:  newCursor = Position(cursor.getX() - 1, cursor.getY()); break;
            case GameCommand::Type::MOVE_RIGHT: newCursor = Position(cursor.getX() + 1, cursor.getY()); break;
            case GameCommand::Type::CONFIRM: return cursor;
            case GameCommand::Type::CANCEL:  return Position(-1, -1);
            default: break;
        }


        if (newCursor.getX() < 0) newCursor = Position(0, newCursor.getY());
        if (newCursor.getY() < 0) newCursor = Position(newCursor.getX(), 0);
        if (newCursor.getX() >= map.getWidth())  newCursor = Position(map.getWidth() - 1, newCursor.getY());
        if (newCursor.getY() >= map.getHeight()) newCursor = Position(newCursor.getX(), map.getHeight() - 1);

        if (newCursor.equals(cursor)) continue;

        cursor = newCursor;
    }

    return originalCursor; 
}


bool Game::playerCastSpell(int spellIndex) {
    PlayerHand& hand = entities->getPlayer().getHand();
    if (spellIndex < 0 || spellIndex >= hand.getSpellCount()) return false;

    ISpell* spell = hand.getSpell(spellIndex);
    int levelSum = entities->getPlayer().getLevel();
    Position target = selectTargetForSpell(spell->getRange(levelSum));

    if (target.getX() == -1) return false;

    bool casted = false;

    if (spell->getName() == "Призыв союзника") {
        casted = hand.useSpell(spellIndex, map, entities->getPlayer().getPosition(), target);
        if (!casted) return false;

        int summonCount = levelSum;
        int summoned = 0;

        if (entities->summonFollower(target, map)) summoned++;

        for (int dx = -1; dx <= 1 && summoned < summonCount; dx++) {
            for (int dy = -1; dy <= 1 && summoned < summonCount; dy++) {
                if (dx == 0 && dy == 0) continue;
                Position p(target.getX() + dx, target.getY() + dy);
                if (map.isPositionValid(p) && entities->summonFollower(p, map)) summoned++;
            }
        }
    } else {
        casted = hand.useSpell(spellIndex, map, entities->getPlayer().getPosition(), target);
    }

    return casted;
}

void Game::saveGame() {
    saveSystem.save(map, entities);
}

void Game::setGameRunning(bool running) { gameRunning = running; }

void Game::startNewGame() {
    level = 1; width = 10; height = 10;
    map = GameMap(width, height);
    delete entities;
    entities = new EntityManager();
    LevelGenerator::generateLevel(map, *entities, map.getWidth(), map.getHeight());
    entities->getPlayer().getHand().addRandomSpell();
    gameRunning = true;
    levelCompleted = false;
    playerDead = false;
}

bool Game::loadGame() {
    delete entities;
    entities = new EntityManager();
    return saveSystem.load(map, entities);
}

void Game::levelUpPlayer(int choice) {
    Player& player = entities->getPlayer();
    visualizer.showLevelUpMenu();
    switch (choice) {
        case 1: player.addMaxHealth(); break;
        case 2: player.addDamage(); break;
        case 3: player.addLevelSpeel(); break;
        default: player.addMaxHealth();
    }
}

void Game::nextLevel() {
    level++;
    if (level > 15) {
        visualizer.showMessage("Победа! Вы прошли все уровни!");
        visualizer.drawMainMenu();
        return;
    }

    visualizer.showLevelUpMenu();
    GameCommand cmd = controller->getMenuCommand();
    int choice = 1;
    switch (cmd.getType()) {
        case GameCommand::Type::MENU_SELECT_1: choice = 1; break;
        case GameCommand::Type::MENU_SELECT_2: choice = 2; break;
        case GameCommand::Type::MENU_SELECT_3: choice = 3; break;
        default: choice = 1;
    }
    levelUpPlayer(choice);

    width++; height++;
    map = GameMap(width, height);
    entities->entityNew();
    LevelGenerator::generateLevel(map, *entities, map.getWidth(), map.getHeight(), level);
    gameRunning = true;
    levelCompleted = false;
}

void Game::showMainMenu() {
    while (!gameRunning) {
        visualizer.drawMainMenu();
        GameCommand cmd = controller->getMenuCommand();

        switch (cmd.getType()) {
            case GameCommand::Type::MENU_SELECT_1:
                startNewGame(); return;
            case GameCommand::Type::MENU_SELECT_2:
                if (loadGame()) {
                    width = map.getWidth();
                    height = map.getHeight();
                    level = map.getWidth() - 9;
                    gameRunning = true;
                    levelCompleted = false;
                    playerDead = false;
                    return;
                }
                visualizer.showMessage("Ошибка загрузки!");
                break;
            case GameCommand::Type::MENU_SELECT_3:
                gameRunning = false; return;
            default:
                visualizer.showMessage("Неправильный выбор!");
        }
    }
}

bool Game::isGameRunning() const { return gameRunning && entities->getPlayer().isAlive() && !entities->areAllEnemiesDead(); }
bool Game::isLevelCompleted() const { return levelCompleted; }
bool Game::isPlayerDead() const { return playerDead; }
GameMap& Game::getMap() { return map; }
EntityManager* Game::getEntities() { return entities; }
int Game::getLevel() const { return level; }

bool Game::initiateSpellCast() {
    PlayerHand& hand = entities->getPlayer().getHand();
    if (hand.getSpellCount() == 0) {
        visualizer.showMessage("Нет заклинаний в руке!");
        return false;
    }

    visualizer.showSpells(hand);
    GameCommand spellCmd = controller->getSpellCommand();
    int spellIndex = -1;
    switch (spellCmd.getType()) {
        case GameCommand::Type::SPELL_0: spellIndex = 0; break;
        case GameCommand::Type::SPELL_1: spellIndex = 1; break;
        case GameCommand::Type::SPELL_2: spellIndex = 2; break;
        case GameCommand::Type::SPELL_3: spellIndex = 3; break;
        case GameCommand::Type::SPELL_4: spellIndex = 4; break;
        case GameCommand::Type::SPELL_5: spellIndex = 5; break;
        case GameCommand::Type::SPELL_6: spellIndex = 6; break;
        case GameCommand::Type::SPELL_7: spellIndex = 7; break;
        case GameCommand::Type::SPELL_8: spellIndex = 8; break;
        case GameCommand::Type::SPELL_9: spellIndex = 9; break;
        default: return false;
    }

    if (spellIndex >= hand.getSpellCount()) return false;

    return playerCastSpell(spellIndex);
}

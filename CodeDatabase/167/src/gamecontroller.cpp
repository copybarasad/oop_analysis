#include "gamecontroller.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

using namespace std;

GameController::GameController()
    : currentState(GameState::MAIN_MENU),
      currentLevel(1),
      isRunning(false)
{
  gameSave = GameSave();
  srand(time(nullptr));
}

GameController::~GameController()
{
  stop();
}

void GameController::start()
{
  isRunning = true;
  mainLoop();
}

void GameController::stop()
{
  isRunning = false;
}

void GameController::clearScreen() const
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

#ifdef _WIN32
char GameController::getCharWithoutEnter() const
{
  return _getch();
}
#else
char GameController::getCharWithoutEnter() const
{
  struct termios oldt, newt;
  char ch;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}
#endif

void GameController::sleepMs(int milliseconds) const
{
#ifdef _WIN32
  Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);
#endif
}

std::string GameController::generateSaveName() const
{

  vector<string> saves = gameSave.listSaves();
  int maxNumber = 0;

  for (const auto &save : saves)
  {
    if (save.find("save_game") == 0)
    {

      string numberStr = save.substr(9);
      try
      {
        int number = stoi(numberStr);
        if (number > maxNumber)
        {
          maxNumber = number;
        }
      }
      catch (...)
      {
      }
    }
  }

  return "save_game" + to_string(maxNumber + 1);
}

void GameController::displayGameStatus() const
{
  if (!player || !gameField)
    return;

  cout << "=== Level " << currentLevel << " ===" << endl;
  cout << "Player: " << player->getName()
       << " | Health: " << player->getHealth() << "/" << player->getMaxHealth()
       << " | Score: " << player->getScore() << endl;

  cout << "Enemies remaining: " << gameField->getEnemies().size()
       << " | Spells: " << player->getHand().getSize()
       << "/" << player->getHand().getMaxSize() << endl
       << endl;
}

void GameController::initializeLevel(int level)
{

  gameField.reset();
  player.reset();

  int fieldSize = 10 + level * 2;
  if (fieldSize > 25)
    fieldSize = 25;

  gameField = unique_ptr<GameField>(new GameField(fieldSize, fieldSize));

  player = unique_ptr<Player>(new Player("Hero", 100 + level * 20, 20 + level * 5, 5));

  int centerX = fieldSize / 2;
  int centerY = fieldSize / 2;
  if (!gameField->placePlayer(player.get(), centerX, centerY))
  {
    throw runtime_error("Failed to place player!");
  }

  int enemyCount = 3 + level * 2;

  for (int i = 0; i < enemyCount; ++i)
  {
    int enemyType = rand() % 4;
    unique_ptr<Enemy> enemy;

    switch (enemyType)
    {
    case 0:
      enemy = unique_ptr<Enemy>(new Enemy("Goblin", 30 + level * 10, 5 + level * 2));
      break;
    case 1:
      enemy = unique_ptr<Enemy>(new Enemy("Orc", 50 + level * 15, 10 + level * 3));
      break;
    case 2:
      enemy = unique_ptr<Enemy>(new Enemy("Skeleton", 25 + level * 8, 8 + level * 2));
      break;
    case 3:
      enemy = unique_ptr<Enemy>(new Enemy("Troll", 80 + level * 20, 15 + level * 4));
      break;
    }

    int x, y;
    do
    {
      x = rand() % fieldSize;
      y = rand() % fieldSize;
    } while (!gameField->isCellEmpty(x, y) ||
             abs(x - centerX) < 3 || abs(y - centerY) < 3);

    gameField->addEnemy(std::move(enemy), x, y);
  }

  currentState = GameState::PLAYING;
}

bool GameController::isLevelComplete() const
{
  if (!gameField || !gameField->isPlayerAlive())
  {
    return false;
  }

  return gameField->getEnemies().empty();
}

void GameController::advanceToNextLevel()
{
  currentLevel++;
  initializeLevel(currentLevel);
}

void GameController::playerTurn(char input)
{
  if (!player || !gameField)
    return;

  if (input == 'w' || input == 'W' || input == 'a' || input == 'A' ||
      input == 's' || input == 'S' || input == 'd' || input == 'D')
  {

    Direction dir = Direction::NONE;
    switch (input)
    {
    case 'w':
    case 'W':
      dir = Direction::UP;
      break;
    case 's':
    case 'S':
      dir = Direction::DOWN;
      break;
    case 'a':
    case 'A':
      dir = Direction::LEFT;
      break;
    case 'd':
    case 'D':
      dir = Direction::RIGHT;
      break;
    }

    if (dir != Direction::NONE)
    {
      gameField->movePlayer(dir);
    }
  }

  else if (input == 'c' || input == 'C')
  {

    cout << "\n=== Cast Spell ===" << endl;
    player->getHand().display();

    if (player->getHand().getSize() == 0)
    {
      cout << "No spells in hand!" << endl;
      return;
    }

    cout << "Select spell (1-" << player->getHand().getSize() << "): ";
    int spellIndex;
    cin >> spellIndex;
    spellIndex--;

    if (spellIndex < 0 || spellIndex >= player->getHand().getSize())
    {
      cout << "Invalid spell selection!" << endl;
      return;
    }

    cout << "Enter target coordinates (x y): ";
    int targetX, targetY;
    cin >> targetX >> targetY;

    player->castSpell(spellIndex, *gameField, targetX, targetY);
  }
}

void GameController::enemyTurn()
{
  if (!gameField)
    return;

  cout << "\n=== Enemy Turn ===" << endl;
  gameField->moveEnemies();

  if (!gameField->isPlayerAlive())
  {
    currentState = GameState::GAME_OVER;
  }
}

void GameController::mainLoop()
{
  while (isRunning)
  {
    switch (currentState)
    {
    case GameState::MAIN_MENU:
    {
      showMainMenu();
      break;
    }

    case GameState::PLAYING:
    {
      clearScreen();
      displayGameStatus();
      gameField->display();

      cout << "=== Controls ===" << endl;
      cout << "W/A/S/D - Move" << endl;
      cout << "C - Cast Spell" << endl;
      cout << "H - Show Hand" << endl;
      cout << "N - Quick Save" << endl;
      cout << "L - Load Menu" << endl;
      cout << "Q - Quit to Menu" << endl;
      cout << "=================" << endl;

      cout << "Enter command: ";
      char input = getCharWithoutEnter();
      cout << input << endl;

      if (input == 'q' || input == 'Q')
      {
        currentState = GameState::MAIN_MENU;
      }
      else if (input == 'n' || input == 'N')
      {

        string saveName = generateSaveName();
        saveGame(saveName);
        sleepMs(1000);
        continue;
      }
      else if (input == 'l' || input == 'L')
      {
        currentState = GameState::LOADING;
      }
      else if (input == 'h' || input == 'H')
      {
        clearScreen();
        player->getHand().display();
        cout << "\nPress any key to return...";
        getCharWithoutEnter();
        continue;
      }
      else
      {
        playerTurn(input);

        if (gameField->isPlayerAlive())
        {
          enemyTurn();
        }

        if (isLevelComplete())
        {
          currentState = GameState::LEVEL_COMPLETE;
        }
        else if (!gameField->isPlayerAlive())
        {
          currentState = GameState::GAME_OVER;
        }

        sleepMs(ACTION_PAUSE_MS);
      }
      break;
    }

    case GameState::LEVEL_COMPLETE:
    {
      showLevelCompleteMenu();
      break;
    }

    case GameState::GAME_OVER:
    {
      showGameOverMenu();
      break;
    }

    case GameState::SAVING:
    {
      showSaveMenu();
      break;
    }

    case GameState::LOADING:
    {
      showLoadMenu();
      break;
    }

    case GameState::EXITING:
    {
      isRunning = false;
      break;
    }
    }
  }
}

void GameController::showMainMenu()
{
  clearScreen();

  cout << "=== RPG GAME ===" << endl;
  cout << "1. New Game" << endl;
  cout << "2. Load Game" << endl;
  cout << "3. Exit" << endl;
  cout << "================" << endl;

  cout << "Select option: ";
  char choice = getCharWithoutEnter();
  cout << choice << endl;

  switch (choice)
  {
  case '1':
    newGame();
    break;
  case '2':
    currentState = GameState::LOADING;
    break;
  case '3':
    currentState = GameState::EXITING;
    break;
  default:
    cout << "Invalid option!" << endl;
    sleepMs(1000);
    break;
  }
}

void GameController::showSaveMenu()
{
  clearScreen();

  cout << "=== Save Game ===" << endl;

  vector<string> saves = gameSave.listSaves();
  if (!saves.empty())
  {
    cout << "Existing saves:" << endl;
    for (const auto &save : saves)
    {
      gameSave.displaySaveInfo(save);
    }
    cout << endl;
  }

  cout << "1. Quick Save (auto name)" << endl;
  cout << "2. Custom Save Name" << endl;
  cout << "3. Cancel" << endl;
  cout << "=================" << endl;

  cout << "Select option: ";
  char choice = getCharWithoutEnter();
  cout << choice << endl;

  switch (choice)
  {
  case '1':
  {
    string saveName = generateSaveName();
    saveGame(saveName);
    break;
  }
  case '2':
  {
    cout << "Enter save name: ";
    string saveName;
    cin >> saveName;
    if (!saveName.empty())
    {
      saveGame(saveName);
    }
    break;
  }
  case '3':

    break;
  default:
    cout << "Invalid option!" << endl;
    sleepMs(1000);
    showSaveMenu();
    return;
  }

  currentState = GameState::PLAYING;
}

void GameController::showLoadMenu()
{
  clearScreen();

  cout << "=== Load Game ===" << endl;

  vector<string> saves = gameSave.listSaves();
  if (saves.empty())
  {
    cout << "No saved games found!" << endl;
    cout << "Press any key to continue...";
    getCharWithoutEnter();
    currentState = GameState::MAIN_MENU;
    return;
  }

  cout << "Available saves:" << endl;
  for (size_t i = 0; i < saves.size(); ++i)
  {
    cout << i + 1 << ". ";
    gameSave.displaySaveInfo(saves[i]);
  }

  cout << "\nSelect save number (or 0 to cancel): ";
  int choice;
  cin >> choice;

  if (choice > 0 && choice <= static_cast<int>(saves.size()))
  {
    loadGame(saves[choice - 1]);
    currentState = GameState::PLAYING;
  }
  else
  {
    currentState = GameState::MAIN_MENU;
  }
}

void GameController::showGameOverMenu()
{
  clearScreen();

  cout << "=== GAME OVER ===" << endl;
  cout << "Your hero has fallen in battle!" << endl;
  cout << "Level reached: " << currentLevel << endl;
  cout << "Final score: " << (player ? player->getScore() : 0) << endl;
  cout << "=========================" << endl;
  cout << "1. Restart Level" << endl;
  cout << "2. New Game" << endl;
  cout << "3. Main Menu" << endl;
  cout << "4. Exit" << endl;
  cout << "=================" << endl;

  cout << "Select option: ";
  char choice = getCharWithoutEnter();
  cout << choice << endl;

  switch (choice)
  {
  case '1':
    restartLevel();
    break;
  case '2':
    newGame();
    break;
  case '3':
    currentState = GameState::MAIN_MENU;
    break;
  case '4':
    currentState = GameState::EXITING;
    break;
  default:
    cout << "Invalid option!" << endl;
    sleepMs(1000);
    showGameOverMenu();
    break;
  }
}

void GameController::showLevelCompleteMenu()
{
  clearScreen();

  cout << "=== LEVEL COMPLETE! ===" << endl;
  cout << "Congratulations! You cleared level " << currentLevel << "!" << endl;
  cout << "Current score: " << (player ? player->getScore() : 0) << endl;
  cout << "=========================" << endl;
  cout << "1. Continue to next level" << endl;
  cout << "2. Save and continue" << endl;
  cout << "3. Save and quit" << endl;
  cout << "4. Main Menu" << endl;
  cout << "=======================" << endl;

  cout << "Select option: ";
  char choice = getCharWithoutEnter();
  cout << choice << endl;

  switch (choice)
  {
  case '1':
    advanceToNextLevel();
    break;
  case '2':
  {
    string saveName = generateSaveName();
    saveGame(saveName);
    advanceToNextLevel();
    break;
  }
  case '3':
  {
    string saveName = generateSaveName();
    saveGame(saveName);
    currentState = GameState::MAIN_MENU;
    break;
  }
  case '4':
    currentState = GameState::MAIN_MENU;
    break;
  default:
    cout << "Invalid option!" << endl;
    sleepMs(1000);
    showLevelCompleteMenu();
    break;
  }
}

void GameController::newGame()
{
  currentLevel = 1;
  initializeLevel(currentLevel);
}

void GameController::restartLevel()
{
  initializeLevel(currentLevel);
}

void GameController::saveGame(const string &saveName)
{
  if (!player || !gameField)
  {
    cout << "Cannot save - no active game!" << endl;
    sleepMs(1500);
    return;
  }

  try
  {
    cout << "Saving game as: " << saveName << "..." << endl;

    if (gameSave.saveGame(saveName, *player, *gameField, currentLevel))
    {
      cout << "Game saved successfully as: " << saveName << endl;
    }
    else
    {
      cout << "Failed to save game!" << endl;
    }
  }
  catch (const exception &e)
  {
    cout << "Error saving game: " << e.what() << endl;
  }

  sleepMs(1500);
}

void GameController::loadGame(const string &saveName)
{
  try
  {
    if (gameSave.loadGame(saveName, *player, *gameField, currentLevel))
    {
      cout << "Game loaded successfully!" << endl;
      currentState = GameState::PLAYING;
    }
    else
    {
      cout << "Failed to load game!" << endl;
      sleepMs(1500);
      currentState = GameState::MAIN_MENU;
    }
  }
  catch (const exception &e)
  {
    cout << "Error loading game: " << e.what() << endl;
    sleepMs(1500);
    currentState = GameState::MAIN_MENU;
  }
}
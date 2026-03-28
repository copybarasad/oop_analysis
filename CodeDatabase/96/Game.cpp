#include "Game.h"

#include "GameView.h"
#include <algorithm>
#include <iostream>

Game::Game():gameManager(nullptr) {
}

void Game::Start() {
    state = "newgame";
    while (true){
        GameView<ConsoleRenderer> view;
        view.print("NEW GAME OR LOAD SAVE");
        GameController<ConsoleInput> controller;
        controller.process(*this);
        if (state == "game"){
            break;
        }
        //system("clear");
    }
    
    GameLoop();
}

void Game::ProcessCommand(Command command, int a1, int a2){
    if (state == "settings"){
        if (command == Command::Exit){
            state = "exit";
            return;
        } else if (command == Command::Save){
            FileStorage storage("../SAVE.txt", Mode::Write);
            savedata data = gameManager->GetDataSave();
            data.currentLevel = currentLevel;
            data.improvehand = improvehand;
            data.improvehealth = improvehealth;
            storage.Save(data);
            return;
        } else if (command == Command::Exit){
            state = "game";
            return;
        } else if (command == Command::Load){
            try{
            FileStorage storage("../SAVE.txt", Mode::Read);
            savedata data = storage.Load();
            currentLevel = data.currentLevel;
            improvehand = data.improvehand;
            improvehealth = data.improvehealth;
            gameManager = std::make_shared<GameManager>(data);
            }
            catch (const std::runtime_error& e) {
                std::cerr << "[Load Error] " << e.what() << std::endl;
                std::cerr << "Загрузка невозможна, используем стартовые данные.\n";
                return;
            }
            catch (...) {
                std::cerr << "Неизвестная ошибка при загрузке.\n";
                return;
            }
        } else if (command == Command::Back){
            state = "game";
        } else {
            return;
        }
    } else if(state == "game"){
        if (command == Command::Settings){
            state = "settings";
            return;
        } else if (command == Command::MoveUp) {
            gameManager->MovePlayer(Direction::Up);
        } else if (command == Command::MoveDown) {
            gameManager->MovePlayer(Direction::Down);
        } else if (command == Command::MoveLeft) {
            gameManager->MovePlayer(Direction::Left);
        } else if (command == Command::MoveRight) {
            gameManager->MovePlayer(Direction::Right);
        } else if (command == Command::AttackMode) {
            gameManager->SwitchAttackMode();
        } else if (command == Command::AttackUp) {
            gameManager->PlayerAttack(Direction::Up);
        } else if (command == Command::AttackDown) {
            gameManager->PlayerAttack(Direction::Down);
        } else if (command == Command::AttackLeft) {
            gameManager->PlayerAttack(Direction::Left);
        } else if (command == Command::AttackRight) {
            gameManager->PlayerAttack(Direction::Right);
        } else if (command == Command::CastSpell) {
            std::pair<int,int> pos;
            pos.first = a1;
            pos.second = a2;
            gameManager->PlayerCastSpell(pos);
            std::cout << "ydachno cast\n";
        } else if (command == Command::SelectSpell) {
            gameManager->SelectSpellInHand(a1);
        }
    } else if (state == "newlevel"){
        if (command == Command::ImproveHealth){
            improvehealth+=1;
            state = "game";
            NextLevel();
        }
        if (command == Command::ImproveHand){
            improvehand+=1;
            state = "game";
            NextLevel();
        } else if(command == Command::None){
            return;
        }
    } else if (state == "newgame"){
        if (command == Command::NewGame){
            state = "game";
            StartNewGame();
        } else if (command == Command::Load){
            try{
            FileStorage storage("../SAVE.txt", Mode::Read);
            savedata data = storage.Load();
            currentLevel = data.currentLevel;
            improvehand = data.improvehand;
            improvehealth = data.improvehealth;
            gameManager = std::make_shared<GameManager>(data);
            state = "game";
            }
            catch (const std::runtime_error& e) {
                std::cerr << "[Load Error] " << e.what() << std::endl;
                std::cerr << "Загрузка невозможна, используем стартовые данные.\n";
                return;
            }
            catch (...) {
                std::cerr << "Неизвестная ошибка при загрузке.\n";
                return;
            }
        } else if (command == Command::None){
            return;
        }
    }
}
 

void Game::GameLoop() {
    //std::string state = "game";
    bool key = true;
    while (key) {
        GameView<ConsoleRenderer> view;
        if (gameManager->GetMurders() >= 5 * currentLevel) {
            state = "newlevel";
        }
        
        if (gameManager->IsGameOver()) {
            view.print("Game Over! You died.");
            key = false;
        }
        view.update(*gameManager, state);

        GameController<ConsoleInput> controller;
        controller.process(*this);
        if (state == "exit"){
            view.print("Goodbye");
            key = false;
        }
        if (state == "game"){
            gameManager->UpdateState();
        }
    }
}

void Game::StartNewGame() {
    currentLevel = 1;
    gameManager = std::make_shared<GameManager>(10, 10, 5, 3, 2);
}


void Game::NextLevel(){
    currentLevel+=1;
    int width = std::min(25, 10 + currentLevel);
    int height = std::min(25, 10 + currentLevel);
    int hp = 100 + 10*(currentLevel-1);
    dataPlayer p = gameManager->GetDataPlayer();
    p.health = 100*improvehealth;
    if (improvehand){
        gameManager->ImproveAllSpells();
    }
    improvehand=0;
    gameManager = std::make_shared<GameManager>(p, gameManager->GetDataHand(), width, height, hp);
}
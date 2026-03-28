#include "game_controller.hpp"

void setNonCanonicalMode() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void restoreTerminalMode() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

GameController::GameController(std::unique_ptr<GameDrawer> game_drawer_ptr) 
    : player(nullptr), hand(nullptr), gameMap(nullptr), current_level(0), gameDrawer(std::move(game_drawer_ptr)) {}


GameController::~GameController() {
    delete player;
    delete hand;
    delete gameMap;
}

void GameController::print_status() {
    /*gameMap->print();
    std::cout << "Hp: " << player->getHp() << "\n";
    hand->print_hand();*/
    gameDrawer->draw_game(gameMap, player, hand);
}

void GameController::load_error(std::string error) {
    std::cout << "Error: " << error << "\n";
    new_game();
}

void GameController::start() {
    std::cout << "New game (N) or Load saved game (L)?\n";
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
            switch (key) {
                case 'n':
                case 'N':
                    new_game();
                    break;
                case 'l':
                case 'L':
                    //load();
                    break;
                default:
                    break;
        }
    }
}

void GameController::new_game() {
    delete player;
    delete hand; 
    delete gameMap;
    player = nullptr;
    hand = nullptr;
    gameMap = nullptr;

    current_level = 1;
    std::cout << "pupupu\n";
    
    hand = new Hand(10);
    hand->addRandomSpell();
    player = new Player(500, 100, 500);
    player->set_hand(hand);
    
    gameMap = new Map(10, 10);

    srand(time(0));
    
    int playerX, playerY;
    do {
        playerX = rand() % gameMap->getWidth();
        playerY = rand() % gameMap->getHeight();
    } while (gameMap->getCellState(playerX, playerY) != CellState::EMPTY);
    
    gameMap->addPlayer(playerX, playerY, player);

    fill_map(1, 1, 100, 10, 300, 50);
    
    print_status();
    //game_cycle();
}

void GameController::gain_stage() {
    std::cout << "You want to gain 50 hp (1) or 20 damage (2)?\n";
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
        switch (key) {
            case '1':
                player->increase_hp();
                break;
            case '2':
                player->increase_damage();
                break;
        }
    }
}

void GameController::next_level(Command com) {
    if (gameMap->getMonsterCount() != 0) {return;}
    current_level += 1;
    //std::cout << "Next level: " << current_level << "\n";

    //gain_stage();
    std::cout << "You want to gain 50 hp (1) or 20 damage (2)?\n";
    if (com.get_text() == "1") {
        player->increase_hp();
    } else if (com.get_text() == "2") {
        player->increase_damage();
    }

    int w = gameMap->getWidth();
    int h = gameMap->getHeight();
    delete gameMap;
    gameMap = new Map(w+1, h+1);
    int playerX, playerY;
    do {
        playerX = rand() % gameMap->getWidth();
        playerY = rand() % gameMap->getHeight();
    } while (gameMap->getCellState(playerX, playerY) != CellState::EMPTY);
    
    gameMap->addPlayer(playerX, playerY, player);
    player->heal_hp();
    hand->remove_half_spells();
    fill_map(current_level, current_level, 100+10*current_level, 10+current_level, 300+30*current_level, 50+5*current_level);
    print_status();
}

void GameController::player_step(Command com) {
    //gameMap->makeStep(gameMap->getPlayerX(), gameMap->getPlayerY());
    std::string str = com.get_text();
    int deltax=0,deltay=0;
    if (str == "w" || str == "W") {
        deltay = -1;
    } else if (str == "a" || str == "A") {
        deltax = -1;
    } else if (str == "s" || str == "S") {
        deltay = 1;
    } else if (str == "d" || str == "D") {
        deltax = 1;
    } else if (str == "m" || str == "M") {
        player->setDamageType(DamageType::MELEE);
    } else if (str == "r" || str == "R") {
        player->setDamageType(DamageType::RANGE);
    } else if (str.length() == 1 && str[0] >= '0' && str[0] <= '9') {
        hand->castSpell(str[0] - '0', gameMap);
    }
    int x=gameMap->getPlayerX(),y=gameMap->getPlayerY();
    int newx=0,newy=0;
    DamageType dtype = player->getDamageType();
    if (deltax+deltay != 0) {
        if (dtype == DamageType::MELEE) {
            newx=x+deltax;
            newy=y+deltay;
            gameMap->moveEntity(x, y, newx, newy);
        } else if (dtype == DamageType::RANGE) {
            int nx=x+deltax;
            int ny=y+deltay;

            while (0 <= nx && nx < gameMap->getWidth() && 0 <= ny && ny < gameMap->getHeight()) {
                if (gameMap->getCellState(nx, ny) == CellState::MONSTER || gameMap->getCellState(nx, ny) == CellState::WALL || gameMap->getCellState(nx, ny) == CellState::TOWER) {
                    //std::cout << "x=\n";
                    gameMap->moveEntity(x, y, nx, ny);
                    break;
                }
                nx+=deltax;
                ny+=deltay;
            }
        }
        
    }
}

void GameController::monsters_step() {
    for (int x=0;x<gameMap->getWidth();x++) {
        for (int y=0;y<gameMap->getHeight();y++) {
            if (gameMap->getCellState(x, y) == CellState::MONSTER) {
                gameMap->makeStep(x, y);
            }
        }
    }
}

void GameController::towers_step() {
    for (int x=0;x<gameMap->getWidth();x++) {
        for (int y=0;y<gameMap->getHeight();y++) {
            if (gameMap->getCellState(x, y) == CellState::TOWER) {
                gameMap->makeStep(x, y);
            }
        }
    }
}

void GameController::save(Command com) {
    restoreTerminalMode();
    std::string filename = com.get_text();
    //std::cout << "In which file you want to save game?\n";
    //std::getline(std::cin, filename);

    sl_master.save(filename, gameMap, hand, current_level);

    

    setNonCanonicalMode();
    std::cout << "Game saved to " << filename << "!\n";
}

void GameController::load(Command com) {
    delete player;
    delete hand; 
    delete gameMap;
    player = nullptr;
    hand = nullptr;
    gameMap = nullptr;

    restoreTerminalMode();
    std::string filename = com.get_text();
    setNonCanonicalMode();
    
    if (sl_master.load(filename, gameMap, hand, player, &current_level)) {
        print_status();
    } else {
        new_game();
    }
}

void GameController::pause_game() {
    std::cout << "Game paused!\nResume (R), Save (S), Load (L)\n";
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
        switch (key) {
            case 'r':
            case 'R':
                break;
            case 's':
            case 'S':
                //save();
                break;
            case 'l':
            case 'L':
                //load();
                break;
    }
    }
}



void GameController::fill_map(int monsters_count, int towers_count, int monsters_hp, int monsters_damage, int towers_hp, int towers_damage) {
    auto placeEntity = [&](std::unique_ptr<Entity> entity, CellState state) {
    int entityX, entityY;
    int playerX = gameMap->getPlayerX();
    int playerY = gameMap->getPlayerY();
    bool validPosition;
    
    do {
        entityX = rand() % gameMap->getWidth();
        entityY = rand() % gameMap->getHeight();
        
        validPosition = true;
        
        if (gameMap->getCellState(entityX, entityY) != CellState::EMPTY) {
            validPosition = false;
            continue;
        }
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = entityX + dx;
                int ny = entityY + dy;
                
                if (nx >= 0 && nx < gameMap->getWidth() && 
                    ny >= 0 && ny < gameMap->getHeight() && 
                    nx == playerX && ny == playerY) {
                    validPosition = false;
                    break;
                }
            }
            if (!validPosition) break;
        }
        
    } while (!validPosition);
    
    gameMap->addEntity(entityX, entityY, std::move(entity), state);
    };


    // Добавляем монстров
    for (int i = 0; i < monsters_count; i++) {
        auto monster = std::make_unique<Monster>(monsters_hp, monsters_damage);
        placeEntity(std::move(monster), CellState::MONSTER);
    }

    // Добавляем башни
    for (int i = 0; i < towers_count; i++) {
        auto tower = std::make_unique<GuardTower>(towers_hp, towers_damage);
        placeEntity(std::move(tower), CellState::TOWER);
    }
}

void GameController::game_cycle(Command com) {
    gameMap->everyoneCanWalk();
    int monsters = gameMap->getMonsterCount();

    player_step(com);
    //print_status();
    /*char key = player->get_last_step();
    switch (key) {
        case 'p':
        case 'P':
            pause_game();
            return;
    }*/

    monsters_step();
    //print_status();

    towers_step();
    //print_status();

    if (gameMap->getMonsterCount() != monsters) {hand->addRandomSpell();}
    print_status();
    gameMap->everyoneCanWalk();
    /*if (player->getHp() == 0) {
        std::cout << "You lose! Want to exit (E), start new game (N) or load saved game (L)?\n";
        char key;
        ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
        if (bytesRead > 0) {
            switch (key) {
                case 'e':
                case 'E':
                    break;
                case 'n':
                case 'N':
                    new_game();
                    break;
                case 'l':
                case 'L':
                    //load();
                    break;
        }
    }
    } else if (gameMap->getMonsterCount() == 0) {
        std::cout << "You won!  Want to exit (E), start new game (N), load saved game (L) or start next level (S)?\n";
        char key;
        ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
        if (bytesRead > 0) {
            switch (key) {
                case 'e':
                case 'E':
                    break;
                case 'n':
                case 'N':
                    new_game();
                    break;
                case 'l':
                case 'L':
                    //load();
                    break;
                case 's':
                case 'S':
                    next_level();
                    break;
            }
        }
    }*/
}

bool GameController::game_active() {
    if (player == nullptr || hand == nullptr || gameMap == nullptr) {
        return false;
    }
    return player->getHp() > 0 && gameMap->getMonsterCount() > 0;
}

void GameController::end_cycle() {
        if (player->getHp() == 0) {
        std::cout << "You lose! Want to exit (E), start new game (N) or load saved game (L)?\n";
        /*char key;
        ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
        if (bytesRead > 0) {
            switch (key) {
                case 'e':
                case 'E':
                    break;
                case 'n':
                case 'N':
                    new_game();
                    break;
                case 'l':
                case 'L':
                    //load();
                    break;
        }
    }*/
    } else if (gameMap->getMonsterCount() == 0) {
        std::cout << "You won!  Want to exit (E), start new game (N), load saved game (L) or start next level (S)?\n";
        /*char key;
        ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
        if (bytesRead > 0) {
            switch (key) {
                case 'e':
                case 'E':
                    break;
                case 'n':
                case 'N':
                    new_game();
                    break;
                case 'l':
                case 'L':
                    //load();
                    break;
                case 's':
                case 'S':
                    next_level();
                    break;
            }
        }*/
    }
}
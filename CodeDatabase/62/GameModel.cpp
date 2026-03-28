#include <iostream>
#include "GameModel.h"
#include "GameEvent.h"
#include "Spell.h"
#include <queue>
#include <cmath>

GameModel::GameModel(int field_width, int field_height, GameEventNotifier* notifier): 
    is_running_(true), 
    scheduledActions_(),
    deck(DECK_SIZE),
    shop(),
    field(field_width, field_height, 1, [this](int d, int x, int y, float del){
        applyDamage(d, x, y, del);
    }), 
    wallet(110),
    goal_(10),
    eventNotifier_(notifier) {
        menu_btns_.push_back(
            ButtonModel("New game", [this](){
                game_state_ = GameState::Running;
                field.clearSpawnerBuildings();
                int w = field.getWidth();
                int h = field.getHeight();
                field = GameField(
                    w,
                    h,
                    1, [this](int d, int x, int y, float del){
                        applyDamage(d, x, y, del);
                    }
                );
                save_game_error_message.clear();
            })
        );
        menu_btns_.push_back(
            ButtonModel("Load game", [this](){
                loadGame();
                if (save_game_error_message.empty()) {
                    game_state_ = GameState::Running;
                }
            })
        );
        menu_btns_.push_back(
            ButtonModel("Exit", [this](){
                is_running_ = false;  
            })
        );

        next_btns_.push_back(
            ButtonModel("Increase max health (10)", [this](){
                Player* p = field.getPlayer();
                if(!p) return;
                if(wallet.spend(10)) {
                    p->setMaxHealth(p->getMaxHealth() + 50);
                }
            })
        );
        next_btns_.push_back(
            ButtonModel("Boost spell (30)", [this](){
                Player* p = field.getPlayer();
                if(!p) return;
                if(wallet.spend(30)) {
                    player_boost_level_++;
                }
            })
        );
        next_btns_.push_back(
            ButtonModel("Increase damage (20)", [this](){
                Player* p = field.getPlayer();
                if(!p) return;
                if(wallet.spend(20)) {
                    p->setDamage(p->getBaseDamage() + 20);
                }
            })
        );
        next_btns_.push_back(
            ButtonModel("Next", [this](){
                nextLevel();
            })
        );
}

void GameModel::update(float dt) {
    processScheduledActions(dt);
    if(game_state_ != GameState::Running) return;
    if(field.getPlayer())
        field.getPlayer()->update(dt);
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); ++j) {
            field.updateCell(j, i, dt);
            if(field.getEnemyAt(j, i) != nullptr) field.getEnemyAt(j, i)->update(dt);
            if(field.getAllyAt(j, i) != nullptr) field.getAllyAt(j, i)->update(dt);
        }
    }

    if(!done_move_) {
        if(!is_player_turn_) {
            bool has_ally = false;
            for(int i = 0; i < field.getHeight(); i++) {
                for(int j = 0; j < field.getWidth(); j++) {
                    if(field.getAllyAt(j, i) != nullptr) {
                        has_ally = true;
                        break;
                    }
                }
            }
            std::cout << "GameModel.cpp: makeAlliesMove" << std::endl;
            makeBotsMove(true);
            
            std::cout << "GameModel.cpp: makeEnemiesMove" << std::endl;
            scheduleAction(has_ally ? 1.0f : 0.1f, [this]() {
                makeBotsMove();
            });
            std::cout << "GameModel.cpp: makeEnemiesMove done" << std::endl;
            std::cout << "GameModel.cpp: makeBuildingsMove" << std::endl;
            field.makeBuildingsMove();
            scheduleAction(has_ally ? 2.0f : 1.0f, [this]() {
                std::cout << "GameModel.cpp: end turn" << std::endl;
                is_player_turn_ = true;
                done_move_ = false;
            });
            resetBotsTurns();
            done_move_ = true;
        } else {
            Player* player = field.getPlayer();
            if(!player) return;
            if(player->getStuckedMoves() > 0) {
                player->unstuck();
                is_player_turn_ = false;
            }
        }
    }
}

void GameModel::movePlayer(float x, float y) {
    if(!is_player_turn_ || done_move_ || game_state_ != GameState::Running) return;
    Player* player = field.getPlayer();
    if(!player) return;
    if(x != 0) {
        player->setDirection(x > 0 ? 1 : -1);
    }
    std::pair<int, int> pos = field.getPlayerCell();
    if(pos.first == -1 || pos.second == -1) return;
    const Enemy* e = field.getEnemyAt(x, y);
    if(field.isFree(pos.first + x, pos.second + y) ) {
        done_move_ = true;
        int fromX = pos.first;
        int fromY = pos.second;
        field.movePlayer(pos.first + x, pos.second + y);
        // Уведомляем о перемещении
        if (eventNotifier_) {
            std::pair<int, int> newPos = field.getPlayerCell();
            MovementEvent event("Player", fromX, fromY, newPos.first, newPos.second);
            eventNotifier_->notify(event);
        }
    } else if(e) {
        done_move_ = true;
        makePlayerAttack(x, y);
    }
    if(done_move_) {
        scheduleAction(2.0f, [this]() {
            std::cout << "GameModel.cpp: end turn" << std::endl;
            is_player_turn_ = false;
            done_move_ = false;
        });
    }
}

void GameModel::changePlayerAttackMode() {
    if(!is_player_turn_ || done_move_ || game_state_ != GameState::Running) return;
    Player* player = field.getPlayer();
    if(!player) return;
    player->toggleRangeAttack();
    // Уведомляем о смене режима атаки
    if (eventNotifier_) {
        GameEvent event(GameEventType::AttackModeChanged);
        event.entityName = "Player";
        event.description = "Player changed attack mode to " + std::string(player->isRangeAttack() ? "ranged" : "melee");
        eventNotifier_->notify(event);
    }
    done_move_ = true;
    scheduleAction(0.5f, [this]() {
        is_player_turn_ = false;
        done_move_ = false;
    });
}

void GameModel::makePlayerAttack(int dx, int dy) {
    std::cout << "GameModel.cpp: makePlayerAttack" << std::endl;
    Player* player = field.getPlayer();
    if(!player) return;
    if(player->getState() != State::Attacking) {
        player->attack();
        std::pair<int, int> playerCell = field.getPlayerCell();
        applyDamage(player->getDamage(), playerCell.first + dx, playerCell.second + dy);
        scheduleAction(0.8f, [this, player]() {
            player->onAttackEnd();
        });
    }
}

std::vector<const Enemy*> GameModel::getEnemies() const {
    std::vector<const Enemy*> v;
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); j++) {
            Enemy* e = field.getEnemyAt(j, i);
            if(e != nullptr) {
                v.push_back(e);
            }
        }
    }
    return v;
}

std::vector<const Entity*> GameModel::getEntities() const {
    std::vector<const Entity*> v;
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); j++) {
            Entity* e = field.getEnemyAt(j, i);
            if(e != nullptr) {
                v.push_back(e);
            }
            Entity* a = field.getAllyAt(j, i);
            if(a != nullptr) {
                v.push_back(a);
            }
        }
    }
    Player* p = field.getPlayer();
    if(p != nullptr) {
        v.push_back(p);
    }
    return v;
}

std::vector<const SpawnerBuilding*> GameModel::getSpawnerBuildings() const {
    std::vector<const SpawnerBuilding*> v;
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); j++) {
            SpawnerBuilding* e = field.getSpawnerBuildingAt(j, i);
            if(e != nullptr) {
                v.push_back(e);
            }
        }
    }
    return v;
}

const Player* GameModel::getPlayer() const {
    return field.getPlayer();
}

void GameModel::playerBoughtItem(const ShopItem* item) {
    deck.addSpell(item->getSpell()->clone());
    // Уведомляем о покупке заклинания
    if (eventNotifier_ && item) {
        SpellEvent event(GameEventType::SpellAcquired, spellTypeToString(item->getSpell()->getName()));
        eventNotifier_->notify(event);
    }
}

void GameModel::handleMouseClick() {
    if(game_state_ == GameState::Menu || game_state_ == GameState::Next || game_state_ == GameState::GameOver){
        if(selected_button_id) {
            for(ButtonModel& b : menu_btns_) {
                if(b.getId() == selected_button_id) {
                    b.click();
                }
            }
            for(ButtonModel& b : next_btns_) {
                if(b.getId() == selected_button_id) {
                    b.click();
                }
            }
        }
        return;
    }
    std::cout << "GameModel.cpp: handleMouseClick" << std::endl;
    if(done_move_ || !is_player_turn_) return;
    if(mouse_on_spell_ != nullptr) {
        if(mouse_on_spell_ == selected_spell_) {
            mouse_on_spell_ = nullptr;
            selected_spell_ = nullptr;
            return;
        }
        selected_spell_ = mouse_on_spell_;
        return;
    }

    Player* player = field.getPlayer();


    if(selected_shop_item_ != nullptr) {
        if(!deck.isFull()) {
            if(wallet.spend(selected_shop_item_->getCost())) {
                // Уведомление о покупке происходит в playerBoughtItem
                playerBoughtItem(selected_shop_item_);
                // Уведомляем о покупке предмета в магазине
                if (eventNotifier_) {
                    GameEvent event(GameEventType::ShopItemPurchased);
                    event.entityName = "Player";
                    event.description = "Player purchased shop item: " + spellTypeToString(selected_shop_item_->getSpell()->getName());
                    event.value = selected_shop_item_->getCost();
                    eventNotifier_->notify(event);
                }
                selected_shop_item_ = nullptr;
            }
        }
        return;
    }


    if(selected_cell_.first < 0 || selected_cell_.second < 0
        || selected_cell_.first >= field.getWidth() || selected_cell_.second >= field.getHeight()) {
        return;
    }

    if(selected_spell_ != nullptr) {
        std::cout << "selected sell: " << selected_cell_.first << " " << selected_cell_.second << std::endl;
        useSpell(selected_spell_);
        return;
    }

    if(!player) return;
    std::pair<int, int> player_cell = field.getPlayerCell();
    bool target = field.getEnemyAt(selected_cell_.first, selected_cell_.second) != nullptr || field.getTowerAt(selected_cell_.first, selected_cell_.second) != nullptr;

    int dist = player_boost_level_ + 1;

    if(player->isRangeAttack() && target && 
        ((selected_cell_.first == player_cell.first && abs(selected_cell_.second - player_cell.second) <= dist) && selected_cell_.second != player_cell.second ||
        (selected_cell_.second == player_cell.second && abs(selected_cell_.first - player_cell.first) <= dist) && selected_cell_.first != player_cell.first)
        ) {
        std::vector<Enemy*> enemies = field.getEnemiesInRow(player_cell.first, player_cell.second, selected_cell_.first, selected_cell_.second);
        if(enemies.size() > 0) {
            player->setDirection(player_cell.first <= selected_cell_.first ? 1 : -1);
            player->attack();
            for(Enemy* e : enemies) {
                std::pair<int, int> pos = field.getEnemyCell(e);
                applyDamage(player->getDamage(), pos.first, pos.second);
            }
            scheduleAction(1.4f, [player]() {
                player->onAttackEnd();
            });
            scheduleAction(2.0f, [this]() {
                is_player_turn_ = false;
                done_move_ = false;
            });
            return;
        }
    }

    std::vector<std::pair<int, int>> path = field.findPath(player_cell.first, player_cell.second, selected_cell_.first, selected_cell_.second);
    if(path.size() == 0) return;

    player->setDirection(player_cell.first <= selected_cell_.first ? 1 : -1);
    
    if(target && path.size() == 1) {
        int dx = selected_cell_.first - player_cell.first;
        int dy = selected_cell_.second - player_cell.second;
        makePlayerAttack(dx, dy);
        scheduleAction(1.0f, [this]() {
            is_player_turn_ = false;
            done_move_ = false;
        });
        return;
    }

    if(!field.getAllyAt(path.front().first, path.front().second) && !field.getTowerAt(path.front().first, path.front().second)) {
        field.movePlayer(path.front().first, path.front().second);
        done_move_ = true;
        scheduleAction(0.5f, [this]() {
            is_player_turn_ = false;
            done_move_ = false;
        });
    }
    return;
}

// Moved implementations from header file

void GameModel::processScheduledActions(float dt) {
    std::vector<size_t> to_remove;
    for (size_t i = 0; i < scheduledActions_.size(); ++i) {
        auto* a = scheduledActions_[i];
        a->triggerTime -= dt;
        if (a->triggerTime <= 0) {
            a->action();
            delete a;
            to_remove.push_back(i);
        }
    }

    // safely erase
    for (int i = (int)to_remove.size() - 1; i >= 0; --i) {
        scheduledActions_.erase(scheduledActions_.begin() + to_remove[i]);
    }
}

void GameModel::makeBotsMove(bool allies) {
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); j++) {
            Enemy* e;
            if(allies) e = field.getAllyAt(j, i);
            else e = field.getEnemyAt(j, i);
            if(e) {
                if(e->isDoneTurn()) continue;
                e->doneTurn();
                std::pair<int, int> player_pos = {-1, -1};
                if(allies) {
                    player_pos = field.getClosestEnemy(j, i);
                } else {
                    player_pos = field.getClosestAllyOrPlayer(j, i);
                }
                if(player_pos.first == -1 || player_pos.second == -1) continue;
                std::vector<std::pair<int, int>> path = field.findPath(j, i, player_pos.first, player_pos.second);
                e->setDirection(player_pos.first > j ? 1 : -1);
                int ex = j, ey = i;
                bool is_allies = allies;
                e->makeTurn(
                    path, 
                    player_pos,
                    [this, ex, ey, is_allies](int x, int y) {
                        Enemy* e;
                        if(is_allies) {
                            e = field.getAllyAt(ex, ey);
                        } else {
                            e = field.getEnemyAt(ex, ey);
                        }
                         
                        std::cout << "Moving enemy to " << x << ", " << y << std::endl;
                        if(!e) return;
                        if(is_allies) {
                            field.moveAlly(e, x, y);
                        } else {
                            field.moveEnemy(e, x, y);
                        }
                    },
                    [this, ex, ey, is_allies](int damage, int x, int y) {
                        std::cout << "Enemy attacking player at " << x << ", " << y << std::endl;
                        scheduleAction(0.8f, [this, ex, ey, is_allies]() { 
                            Enemy* e;
                            if(is_allies) {
                                e = field.getAllyAt(ex, ey);
                            } else {
                                e = field.getEnemyAt(ex, ey);
                            }
                            std::cout << "Enemy attacking player at " << ex << ", " << ey << std::endl;
                            if(!e) return;
                            e->onAttackEnd();
                        });
                        applyDamage(damage, x, y);
                    });
            }
        }
    }
}

void GameModel::resetBotsTurns() {
    for(int i = 0; i < field.getHeight(); i++) {
        for(int j = 0; j < field.getWidth(); j++) {
            Enemy* e = field.getEnemyAt(j, i);
            if(e) {
                e->resetTurn();
            }
            Enemy* a = field.getAllyAt(j, i);
            if(a) {
                a->resetTurn();
            }
        }
    }
}

void GameModel::addScore(int v) {
    score_ += v;
    if(score_ >= goal_) {
        finishLevel();
    }
}

void GameModel::finishLevel() {
    game_state_ = GameState::Next;
}

void GameModel::nextLevel() {
    Player* p = field.getPlayer();
    if(!p) return;
    p->applyHeal(p->getMaxHealth());
    int needSize = deck.getActualSize() / 2;
    int index = 0;
    while(deck.getActualSize() > needSize) {
        if(rand() % 2 == 0) {
            deck.removeSpell(index);
        }
        index++;
        if(index >= deck.getActualSize()) {
            index = 0;
        }
    }
    int w = field.getWidth();
    int h = field.getHeight();
    int level = field.getLevel();
    field = GameField(
        h,
        w,
        level + 1,
        [this](int d, int x, int y, float del){
        applyDamage(d, x, y, del);
        }
    );
    score_ = 0;
    goal_ +=5;
    game_state_ = GameState::Running;
    is_player_turn_ = true;
    done_move_ = false;
}

void GameModel::saveGame() {
    save_game_error_message = "";
    try {
        if(!is_player_turn_ || done_move_ || game_state_ != GameState::Running) {
            throw GameSaveException("You can only save when it's your turn");
        }
        std::cout << "GameModel.cpp: saveGame" << std::endl;
        std::ofstream saveFile(SAVE_FILE);
        if (!saveFile.is_open()) {
            throw GameSaveException("Failed to open save file for writing");
        }

        saveFile << "[PLAYER]\n";
        // Game state
        saveFile << player_boost_level_ << " " 
            << goal_ << " " 
            << score_ << "\n";

        // Player info
        const Player* player = field.getPlayer();
        if(!player) throw GameSaveException("Player not found");
            std::pair<int, int> pos = field.getPlayerCell();
            saveFile << pos.first << " " 
                    << pos.second << " "
                    << player->getHealth() << " " 
                    << player->getMaxHealth() << " "
                    << player->getBaseDamage() << " " 
                    << player->getDirection() << " "
                    << player->isRangeAttack() << " " 
                    << player->getStuckedMoves() << " "
                    << player->getSpeed() << "\n";

        // Field info
        saveFile << "[FIELD]\n";

        int w = field.getWidth(), h = field.getHeight(), level = field.getLevel();
        saveFile << w << " " 
            << h << " " 
            << level << "\n";

        // Grid data
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // Cell info
                saveFile << "[CELL at " << x << ", " << y << "]\n";

                saveFile << field.getCell(x, y) << " " 
                    << field.getTrap(x, y) << "\n";
                
                // Enemy
                const Enemy* e = field.getEnemyAt(x, y);
                saveFile << (e != nullptr) << "\n";
                if (e) {
                    saveFile << e->getHealth() << " " << e->getMaxHealth() << " "
                             << e->getBaseDamage() << " " << e->getDirection() << " " 
                             << e->getSpeed() << "\n";
                }
                
                // Ally
                const Enemy* a = field.getAllyAt(x, y);
                saveFile << (a != nullptr) << "\n";
                if (a) {
                    saveFile << a->getHealth() << " " << a->getMaxHealth() << " "
                             << a->getBaseDamage() << " " << a->getDirection() << " "
                             << a->getSpeed() << "\n";
                }
                
                // Tower
                const Tower* t = field.getTowerAt(x, y);
                saveFile << (t != nullptr) << "\n";
                if (t) {
                    saveFile << t->getHealth() << " " 
                        << t->getMaxHealth() << " "  
                        << t->getTurnsToAttack() << " " 
                        << t->getLastedTurns() << " "
                        << t->getRadius() << "\n";
                    Spell* s = t->getSpell();
                    saveFile << static_cast<int>(s->getName()) << " " 
                        << s->getRadius() << " "
                        << s->getValue() << " "
                        << s->getDist() << "\n";
                }
                
                // Building
                const SpawnerBuilding* b = field.getSpawnerBuildingAt(x, y);
                saveFile << (b != nullptr) << "\n";
                if (b) {
                    saveFile << b->getName() << " "
                        << b->getTurnsToSpawn() << " "
                        << b->getLastedTurns() << "\n";
                }
                
                saveFile << "\n";
            }
        }

        // Wallet
        saveFile << wallet.getBalance() << "\n";

        // Spells
        const std::vector<const Spell*> spells = deck.getSpells();
        saveFile << spells.size() << "\n";
        for (const Spell* s : spells) {
            if (s) {
                saveFile << static_cast<int>(s->getName()) << " "
                         << s->getRadius() << " " << s->getValue() << " " << s->getDist() << "\n";
            }
        }

        saveFile.close();
        if (saveFile.fail()) {
            throw GameSaveException("Failed to write save data");
        }
        
        // Уведомляем об успешном сохранении
        if (eventNotifier_) {
            GameEvent event(GameEventType::GameSaved);
            event.entityName = "Player";
            event.description = "Game saved successfully";
            eventNotifier_->notify(event);
        }
    } catch (const std::exception& e) {
        save_game_error_message = e.what();
        std::cerr << "Save error: " << e.what() << std::endl;
    }
}

void GameModel::loadGame() {
    save_game_error_message = "";
    try {
        if(!is_player_turn_ || done_move_ || (
            game_state_ != GameState::Running &&
            game_state_ != GameState::GameOver &&
            game_state_ != GameState::Menu
        )) {
            throw GameLoadException("You can only load game in your turn");
        }
        std::ifstream loadFile(SAVE_FILE);
        std::cout << "GameModel.cpp: loadGame" << std::endl;
        if (!loadFile.is_open()) {
            std::cout << "Failed to open save file for reading" << std::endl;
            throw GameLoadException("Failed to open save file for reading");
        }

        std::string line;
        getline(loadFile, line);
        if(line != "[PLAYER]") {
            throw GameLoadException("Invalid save file format: missing [PLAYER] signature");
        }

        // Очистка текущего состояния
        game_state_ = GameState::Running;
        is_player_turn_ = true;
        done_move_ = false;
        
        // Game state
        loadFile >> player_boost_level_ 
                >> goal_ 
                >> score_;

        std::cout << "Game state: " << (is_player_turn_ ? "Player's turn" : "Bot's turn") 
                  << ", boost: " << player_boost_level_ 
                  << ", score: " << score_ << "/" << goal_ << std::endl;

        // Player info
        int px, py, ph, pmh, pd, pdir, ps;
        bool pra;
        float pspeed;
        loadFile >> px >> py >> ph >> pmh >> pd >> pdir >> pra >> ps >> pspeed;
        
        Player* player = new Player("crystalKing", pspeed, pmh, pd);
        player->applyDamage(pmh - ph);
        player->setDirection(pdir);
        player->setRangeAttack(pra);
        for (int i = 0; i < ps; i++) player->stuck();

        std::cout << "Player at: " << px << "," << py << " health: " << ph << "/" << pmh << std::endl;

        // Field info 
        getline(loadFile, line); 
        getline(loadFile, line); 
        std::cout << "line: " << line << std::endl;
        if(line != "[FIELD]") {
            throw GameLoadException("Invalid save file format: missing [FIELD] signature");
        }
        int w, h, level;
        loadFile >> w >> h >> level;
        std::cout << "Field size: " << w << "x" << h << " level: " << level << std::endl;
        field = GameField(w, h, level, [this](int d, int x, int y, float del) {
            applyDamage(d, x, y, del);
        });

        if(pspeed < 0) {
            throw GameLoadException("Invalid save file format: invalid player speed");
        }

        if(px < 0 || px >= w || py < 0 || py >= h) {
            throw GameLoadException("Invalid save file format: invalid player position");
        }

        if(w <= 0 || h <= 0 || level <= 0) {
            throw GameLoadException("Invalid save file format: invalid field size or level");
        }

        std::cout << "Field size: " << w << "x" << h << " level: " << level << std::endl;

        // Grid data - пропускаем остаток строки после level
        std::getline(loadFile, line); // пропустить оставшуюся часть строки

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // 1. Читаем и проверяем заголовок ячейки
                std::getline(loadFile, line);
                // std::cout << "Cell header: " << line << std::endl;
                if (line.substr(0, 8) != "[CELL at") {
                    throw GameLoadException("Invalid save cell file format: missing cell header");
                }

                // 2. Читаем и парсим строку с типом клетки и ловушкой
                std::getline(loadFile, line);
                // std::cout << "Cell data: " << line << std::endl;
                std::istringstream cellStream(line);
                std::string cellTypeStr;
                int trapValue;
                cellStream >> cellTypeStr >> trapValue;
                // std::cout << "Cell type: '" << cellTypeStr << "', trap: " << trapValue << std::endl;
                
                char cellType = cellTypeStr.empty() ? ' ' : cellTypeStr[0];
                field.recreateCell(cellType, x, y);
                if (trapValue > 0) {
                    field.installTrapAt(x, y, trapValue);
                }

                // 3. Читаем флаг врага
                std::getline(loadFile, line);
                // std::cout << "Enemy flag: " << line << std::endl;
                std::istringstream enemyFlagStream(line);
                bool hasEnemy;
                enemyFlagStream >> hasEnemy;
                // std::cout << "Has enemy: " << hasEnemy << std::endl;
                
                if (hasEnemy) {
                    // Читаем следующую строку с данными врага
                    std::getline(loadFile, line);
                    // std::cout << "Enemy data: " << line << std::endl;
                    std::istringstream enemyDataStream(line);
                    int eh, emh, ed, edir;
                    float espeed;
                    enemyDataStream >> eh >> emh >> ed >> edir >> espeed;
                    // std::cout << "Enemy stats - health: " << eh << "/" << emh << ", damage: " << ed << ", dir: " << edir << std::endl;

                    if(espeed < 0) {
                        throw GameLoadException("Invalid save file format: invalid enemy speed");
                    }
                    
                    Enemy* enemy = new Enemy("fireKing", espeed, emh, ed);
                    enemy->applyDamage(emh - eh);
                    enemy->setDirection(edir);
                    field.addEnemy(enemy, x, y);
                    // std::cout << "Added enemy at " << x << "," << y << std::endl;
                }

                // 4. Читаем флаг союзника
                std::getline(loadFile, line);
                // std::cout << "Ally flag: " << line << std::endl;
                std::istringstream allyFlagStream(line);
                bool hasAlly;
                allyFlagStream >> hasAlly;
                // std::cout << "Has ally: " << hasAlly << std::endl;
                
                if (hasAlly) {
                    // Читаем следующую строку с данными союзника
                    std::getline(loadFile, line);
                    // std::cout << "Ally data: " << line << std::endl;
                    std::istringstream allyDataStream(line);
                    int ah, amh, ad, adir;
                    float aspeed;
                    allyDataStream >> ah >> amh >> ad >> adir >> aspeed;
                    // std::cout << "Ally stats - health: " << ah << "/" << amh << ", damage: " << ad << ", dir: " << adir << std::endl;

                    if(aspeed < 0) {
                        throw GameLoadException("Invalid ally speed");
                    }
                    
                    Enemy* ally = new Enemy("crystalKing", aspeed, amh, ad);
                    ally->applyDamage(amh - ah);
                    ally->setDirection(adir);
                    field.addAlly(ally, x, y);
                    // std::cout << "Added ally at " << x << "," << y << std::endl;
                }

                // 5. Читаем флаг башни
                std::getline(loadFile, line);
                // std::cout << "Tower flag: " << line << std::endl;
                std::istringstream towerFlagStream(line);
                bool hasTower;
                towerFlagStream >> hasTower;
                // std::cout << "Has tower: " << hasTower << std::endl;

                if (hasTower) {
                    // Читаем первую строку с данными башни
                    std::getline(loadFile, line);
                    // std::cout << "Tower stats: " << line << std::endl;
                    std::istringstream towerStatsStream(line);
                    
                    int th, tmh, turns_to_attack, lasted_turns, radius;
                    towerStatsStream >> th >> tmh >> turns_to_attack >> lasted_turns >> radius;
                    
                    // std::cout << "Tower stats - health: " << th << "/" << tmh 
                    //         << ", turns: " << turns_to_attack 
                    //         << ", radius: " << radius << std::endl;
                    
                    // Читаем ВТОРУЮ строку с данными заклинания
                    std::getline(loadFile, line);
                    // std::cout << "Tower spell: " << line << std::endl;
                    std::istringstream spellStream(line);
                    
                    int spell_type, r, v, d;
                    spellStream >> spell_type >> r >> v >> d;
                    
                    // std::cout << "Spell type: " << spell_type 
                    //         << ", params: " << r << " " << v << " " << d << std::endl;
                    
                    Spell* spell = nullptr;
                    switch (spell_type) {
                        case 0: spell = new InstantDamageSpell(r, v, d); break;
                        case 1: spell = new HealthSpell(r, v, d); break;
                        case 2: spell = new SplashDamageSpell(r, v, d); break;
                        case 3: spell = new TrapSpell(r, v, d); break;
                        case 4: spell = new BoostSpell(r, v, d); break;
                        case 5: spell = new AllySpell(r, v, d); break;
                        default:
                            // std::cout << "Unknown spell type: " << spell_type << std::endl;
                            throw GameLoadException("Unknown spell type");
                            break;
                    }

                    if (spell) {
                        Tower* tower = new Tower(spell, 
                            [this](int damage, int x, int y, float delay) {
                                applyDamage(damage, x, y, delay);
                            }, turns_to_attack, radius, tmh);
                        tower->applyDamage(tmh - th);
                        tower->setLastedTurns(lasted_turns);
                        field.addTower(tower, x, y);
                        // std::cout << "Added tower at " << x << "," << y << std::endl;
                    }
                }

                // 6. Читаем флаг здания
                std::getline(loadFile, line);
                // std::cout << "Building flag: " << line << std::endl;
                std::istringstream buildingFlagStream(line);
                bool hasBuilding;
                buildingFlagStream >> hasBuilding;
                // std::cout << "Has building: " << hasBuilding << std::endl;
                
                if (hasBuilding) {
                    // Читаем следующую строку с данными здания
                    std::getline(loadFile, line);
                    // std::cout << "Building data: " << line << std::endl;
                    std::istringstream buildingDataStream(line);
                    
                    std::string buildingName;
                    int turns_to_spawn, lasted_turns;
                    buildingDataStream >> buildingName >> turns_to_spawn >> lasted_turns;
                    // std::cout << "Building: " << buildingName 
                    //         << ", turns: " << turns_to_spawn 
                    //         << ", lasted: " << lasted_turns << std::endl;
                    
                    SpawnerBuilding* building = new SpawnerBuilding(buildingName,
                        [this, buildingName](GameField* f) {
                            f->spawnEntity(buildingName);
                        }, turns_to_spawn);
                    building->setLastedTurns(lasted_turns);
                    field.addBuilding(building, x, y);
                    // std::cout << "Added building at " << x << "," << y << std::endl;
                }

                // 7. Пропускаем пустую строку между ячейками (если есть)
                if (loadFile.peek() == '\n') {
                    std::getline(loadFile, line);
                    // std::cout << "Skipped empty line" << std::endl;
                }
                
                // std::cout << "Finished processing cell " << x << "," << y << std::endl;
                // std::cout << "----------------------------------------" << std::endl;
            }
        }
        field.addPlayer(player, px, py);
        // Wallet
        int walletBalance;
        loadFile >> walletBalance;

        if(walletBalance < 0) {
            throw GameLoadException("Invalid save file format: wallet balance is negative");
        }

        wallet.setBalance(walletBalance);
        std::cout << "Wallet balance: " << walletBalance << std::endl;

        // Spells
        size_t spellCount;
        loadFile >> spellCount;
        deck.clear();
        std::cout << "Spell count: " << spellCount << std::endl;

        for (size_t i = 0; i < spellCount; i++) {
            int spellTypeInt, spellRadius, spellValue, spellDist;
            if (loadFile >> spellTypeInt >> spellRadius >> spellValue >> spellDist) {
                Spell* spell = nullptr;
                switch (spellTypeInt) {
                    case 0: spell = new InstantDamageSpell(spellRadius, spellValue, spellDist); break;
                    case 1: spell = new HealthSpell(spellRadius, spellValue, spellDist); break;
                    case 2: spell = new SplashDamageSpell(spellRadius, spellValue, spellDist); break;
                    case 3: spell = new TrapSpell(spellRadius, spellValue, spellDist); break;
                    case 4: spell = new BoostSpell(spellRadius, spellValue, spellDist); break;
                    case 5: spell = new AllySpell(spellRadius, spellValue, spellDist); break;
                    default: break;
                }
                
                if (spell) {
                    deck.addSpell(spell);
                    std::cout << "Added spell type " << spellTypeInt << std::endl;
                } else {
                    std::cout << "Unknown spell type " << spellTypeInt << std::endl;
                    throw GameLoadException("Invalid save file format: unknown spell type");
                }
            }
        }
        std::cout << "Deck size after load: " << deck.getSize() << std::endl;

        loadFile.close();
        if(loadFile.fail()) {
            throw GameLoadException("Failed to close save data");
        }
        
        // Уведомляем об успешной загрузке
        if (eventNotifier_) {
            GameEvent event(GameEventType::GameLoaded);
            event.entityName = "Player";
            event.description = "Game loaded successfully";
            eventNotifier_->notify(event);
        }
        
    } catch (const std::exception& e) {
        save_game_error_message = e.what();
        std::cerr << "Load error: " << e.what() << std::endl;
    }
}

void GameModel::applyDamage(int damage, int x, int y, float delay) {
    std::cout << "GameModel.h:159: applyDamage(" << damage << ", " << x << ", " << y << ", " << delay << ")" << std::endl;
    Enemy* target = field.getEnemyAt(x, y);
    if(target != nullptr) {
        scheduleAction(delay, [x, y, this, damage]() {
            Enemy* target = field.getEnemyAt(x, y);
            std::cout << "Applying damage to enemy at " << x << ", " << y << std::endl;
            if(target == nullptr) return;
            // Уведомляем об уроне
            if (eventNotifier_) {
                DamageEvent dmgEvent("Player", "Enemy", damage, x, y);
                eventNotifier_->notify(dmgEvent);
            }
            target->applyDamage(damage);
            if(target->getHealth() <= 0) {
                // Уведомляем о смерти врага
                if (eventNotifier_) {
                    EntityDeathEvent deathEvent("Enemy", "Enemy", x, y);
                    eventNotifier_->notify(deathEvent);
                }
                wallet.add(10);
                addScore(10);
                field.removeEnemy(target);
            }
        });
    }
    Enemy* ally_target = field.getAllyAt(x, y);
    if(ally_target != nullptr) {
        scheduleAction(delay, [x, y, this, damage]() {
            Enemy* ally_target = field.getAllyAt(x, y);
            std::cout << "Applying damage to ally at " << x << ", " << y << std::endl;
            if(ally_target == nullptr) return;
            ally_target->applyDamage(damage);
            if(ally_target->getHealth() <= 0) {
                field.removeAlly(ally_target);
            }
        });
    }

    Tower* tower = field.getTowerAt(x, y);
    if(tower != nullptr) {
        scheduleAction(delay, [x, y, this, damage]() {
            Tower* tower = field.getTowerAt(x, y);
            std::cout << "Applying damage to tower at " << x << ", " << y << std::endl;
            if(tower == nullptr) return;
            // Уведомляем об уроне по башне
            if (eventNotifier_) {
                DamageEvent dmgEvent("Player", "Tower", damage, x, y);
                eventNotifier_->notify(dmgEvent);
            }
            tower->applyDamage(damage);
            if(tower->getHealth() <= 0) {
                // Уведомляем о разрушении башни
                if (eventNotifier_) {
                    EntityDeathEvent deathEvent("Tower", "Tower", x, y);
                    eventNotifier_->notify(deathEvent);
                }
                wallet.add(30);
                addScore(30);
                field.removeTower(tower);
            }
        });
    }

    std::pair<int, int> pos = field.getPlayerCell();
    Player* player = field.getPlayer();
    if(player != nullptr && pos.first == x && pos.second == y) {
        scheduleAction(delay, [this, damage]() {
            Player* player = field.getPlayer();
            std::cout << "Applying damage to player" << std::endl;
            if(player == nullptr) return;
            player->applyDamage(damage);
            if(player->getHealth() <= 0) {
                field.removePlayer();
                game_state_ = GameState::GameOver;
            }
        });
    }
}

void GameModel::scheduleAction(float triggerTime, std::function<void()> action) {
    ScheduledAction* scheduledAction = new ScheduledAction();
    scheduledAction->triggerTime = triggerTime;
    scheduledAction->action = action;
    scheduledActions_.push_back(scheduledAction);
}

void GameModel::selectCell(int x, int y) {
    selected_cell_ = {x, y};
}

void GameModel::useSpell(Spell* spell) {
    if(!spell) return;

    int x = selected_cell_.first;
    int y = selected_cell_.second;
    if(x < 0) x = 0;
    if(x > field.getWidth() - 1) x = field.getWidth() - 1;
    if(y < 0) y = 0;
    if(y > field.getHeight() - 1) y = field.getHeight() - 1;
    std::pair<int, int> player_pos = field.getPlayerCell();
    if(player_pos.first < 0 || player_pos.second < 0) return;

    int dist = std::ceil(hypot(x - player_pos.first, y - player_pos.second));

    std::cout << "Using spell " << spellTypeToString(spell->getName()) << " at " << x << ", " << y <<  ", dist: " << dist << std::endl;

    if(spell->use(field, x, y, dist, player_boost_level_)) {
        // Уведомляем об использовании заклинания
        if (eventNotifier_) {
            SpellEvent event(GameEventType::SpellUsed, spellTypeToString(spell->getName()), x, y);
            eventNotifier_->notify(event);
        }
        selected_spell_ = nullptr;
        mouse_on_spell_ = nullptr;
        deck.removeSpell(spell);

        done_move_ = true;
        scheduleAction(0.5f, [this]() {
            is_player_turn_ = false;
            done_move_ = false;
        });
    }
}

void GameModel::selectSpell(Spell* spell) { 
    if(selected_spell_ == spell) {
        selected_spell_ = nullptr;
    } else {
        selected_spell_ = spell;
    }
}

void GameModel::selectShopItem(const ShopItem* item) {
    selected_shop_item_ = item;
}

void GameModel::processEvents(const std::vector<Event>& events) {
    for (const Event& event : events) {
        switch (event.getType()) {
            case EventType::MovePlayer:
                if (event.getFloatArgsCount() >= 2) {
                    movePlayer(event.getFloatArg(0), event.getFloatArg(1));
                }
                break;
            case EventType::ChangePlayerAttackMode:
                changePlayerAttackMode();
                break;
            case EventType::HandleMouseClick:
                handleMouseClick();
                break;
            case EventType::SaveGame:
                saveGame();
                break;
            case EventType::LoadGame:
                loadGame();
                break;
            case EventType::UseSpellFromHand:
                if (event.getIntArgsCount() >= 1) {
                    useSpellFromHand(event.getIntArg(0));
                }
                break;
            case EventType::BuyShopItem:
                if (event.getIntArgsCount() >= 1) {
                    buyShopItem(event.getIntArg(0));
                }
                break;
            case EventType::AttackDirection:
                if (event.getFloatArgsCount() >= 2) {
                    attackDirection(event.getFloatArg(0), event.getFloatArg(1));
                }
                break;
            case EventType::SelectButton:
                if (event.getIntArgsCount() >= 1) {
                    selectButton(event.getIntArg(0));
                }
                break;
        }
    }
}

void GameModel::useSpellFromHand(int index) {
    if(!is_player_turn_ || done_move_ || game_state_ != GameState::Running) return;
    if(index < 0 || index >= deck.getActualSize()) return;
    
    Spell* spell = deck.getSpell(index);
    if(!spell) return;
    
    std::pair<int, int> player_pos = field.getPlayerCell();
    if(player_pos.first < 0 || player_pos.second < 0) return;
    
    int targetX = -1, targetY = -1;
    SpellType spellType = spell->getName();
    
    // Определяем цель в зависимости от типа заклинания
    if(spellType == SpellType::Health || spellType == SpellType::Boost) {
        // Для лечения и буста - применяем на игрока
        targetX = player_pos.first;
        targetY = player_pos.second;
    } else if(spellType == SpellType::Ally) {
        // Для союзника - применяем на ближайшую пустую клетку
        std::pair<int, int> freePos = field.getFreePosition();
        if(freePos.first != -1 && freePos.second != -1) {
            targetX = freePos.first;
            targetY = freePos.second;
        } else {
            // Если нет свободных клеток, применяем рядом с игроком
            targetX = player_pos.first;
            targetY = player_pos.second;
        }
    } else {
        // Для урона, ловушек и сплэш-урона - применяем на ближайшего врага
        std::pair<int, int> enemyPos = field.getClosestEnemy(player_pos.first, player_pos.second);
        if(enemyPos.first != -1 && enemyPos.second != -1) {
            targetX = enemyPos.first;
            targetY = enemyPos.second;
        } else {
            // Если нет врагов, применяем на пустую клетку рядом с игроком
            targetX = player_pos.first;
            targetY = player_pos.second;
        }
    }
    
    if(targetX < 0 || targetY < 0) return;
    
    int dist = std::ceil(hypot(targetX - player_pos.first, targetY - player_pos.second));
    
    if(spell->use(field, targetX, targetY, dist, player_boost_level_)) {
        // Уведомляем об использовании заклинания
        if (eventNotifier_) {
            SpellEvent event(GameEventType::SpellUsed, spellTypeToString(spell->getName()), targetX, targetY);
            eventNotifier_->notify(event);
        }
        deck.removeSpell(index);
        done_move_ = true;
        scheduleAction(0.5f, [this]() {
            is_player_turn_ = false;
            done_move_ = false;
        });
    }
}

void GameModel::buyShopItem(int index) {
    if(game_state_ != GameState::Running) return;
    if(index < 0 || index >= shop.getSize()) return;
    if(deck.isFull()) return;
    
    const ShopItem* item = shop.getItem(index);
    if(!item) return;
    
    if(wallet.spend(item->getCost())) {
        playerBoughtItem(item);
        // Уведомляем о покупке предмета в магазине
        if (eventNotifier_) {
            GameEvent event(GameEventType::ShopItemPurchased);
            event.entityName = "Player";
            event.description = "Player purchased shop item: " + spellTypeToString(item->getSpell()->getName());
            event.value = item->getCost();
            eventNotifier_->notify(event);
        }
    }
}

void GameModel::attackDirection(float x, float y) {
    if(!is_player_turn_ || done_move_ || game_state_ != GameState::Running) return;
    Player* player = field.getPlayer();
    if(!player) return;
    
    std::pair<int, int> player_pos = field.getPlayerCell();
    if(player_pos.first < 0 || player_pos.second < 0) return;
    
    int dx = (int)x;
    int dy = (int)y;
    
    makePlayerAttack(dx, dy);
    done_move_ = true;
    scheduleAction(2.0f, [this]() {
        is_player_turn_ = false;
        done_move_ = false;
    });
}

void GameModel::selectButton(int index) {
    if(index < 0) return;
    
    // Устанавливаем выбранную кнопку
    setSelectedBtnId(index + 1); // +1 потому что ID начинается с 1
    
    // Если это меню или экран следующего уровня, сразу нажимаем кнопку
    if(game_state_ == GameState::Menu) {
        if(index < menu_btns_.size()) {
            menu_btns_[index].click();
        }
    } else if(game_state_ == GameState::Next) {
        if(index < next_btns_.size()) {
            next_btns_[index].click();
        }
    } else if(game_state_ == GameState::GameOver) {
        // В случае GameOver можно обработать отдельно, если нужно
    }
}
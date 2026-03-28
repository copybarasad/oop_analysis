#include "game.h"
#include "lightning_spell.h"
#include "fireball_spell.h"
#include "trap_spell.h"
#include "game_save.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#define start_map_size 10

void game::add_log(const std::string &msg)
{
    message_log.push_back(msg);
    if (message_log.size() > 7)
    {
        message_log.erase(message_log.begin());
    }
}

const std::vector<std::string> &game::get_log() const
{
    return message_log;
}

void game::saveGameToFile(const std::string &filename)
{
    save_system.saveGame(this, filename);
}


void game::loadGameDuringPlay(const std::string &filename)
{

    game *temp_game = new game();

    try
    {
        std::cout << "Attempting to load game...\n";
        save_system.loadGame(temp_game, filename);

        // if okay
        this->cleanup();

        this->field_ptr = temp_game->field_ptr;
        this->player_ptr = temp_game->player_ptr;
        this->enemies = temp_game->enemies;
        this->towers = temp_game->towers;

        this->current_level = temp_game->current_level;
        this->turn = temp_game->turn;
        this->is_game_over = temp_game->is_game_over;
        this->next_enemy_id = temp_game->next_enemy_id;

        temp_game->field_ptr = nullptr;
        temp_game->player_ptr = nullptr;
        temp_game->enemies.clear();
        temp_game->towers.clear();

        delete temp_game;

        std::cout << "Game loaded successfully!\n";
        std::cout << "Resuming game...\n";
    }
    catch (const SaveGameException &e)
    {
        // if not okay
        std::cerr << "Error loading game: " << e.what() << "\n";
        std::cout << "Returning to current game...\n";
        delete temp_game;
    }
    field_ptr->print_field();
}

void game::nextLevel()
{ // done

    if (current_level >= 3)
    {
        std::cout << "\n=== CONGRATULATIONS! YOU BEAT THE GAME! ===\n";
        is_game_over = true;
        return;
    }

    std::cout << "\n=== LEVEL COMPLETE! ===\n";

    // level count (3 = game ends)
    current_level++;

    // spells remove done
    Hand *hand = player_ptr->get_hand();
    int spells_to_remove = hand->getSpellCount() / 2;
    std::cout << "You lose " << spells_to_remove << " spells.\n";

    for (int i = 0; i < spells_to_remove; i++)
    {
        if (hand->getSpellCount() > 0)
        {
            std::uniform_int_distribution<> spell_dist(0, hand->getSpellCount() - 1);
            int index = spell_dist(gen);
            hand->removeSpell(index);
        }
    }

    // player heal
    player_ptr->set_health(player_ptr->get_max_health_points());

    // Menu done
    levelUpMenu();

    // new map done
    delete field_ptr;
    int new_size = start_map_size + (current_level - 1);
    field_ptr = new game_field(new_size, new_size);

    // vector clearing
    for (auto e : enemies)
        delete e;
    enemies.clear();
    for (auto t : towers)
        delete t;
    towers.clear();

    // reset player done
    player_ptr->set_position(0, 0, field_ptr);

    // new enemies done
    int enemies_count = 3 + current_level;
    for (int i = 0; i < enemies_count; i++)
    {
        spawnEnemy();
        Enemy *new_enemy = enemies.back();
        new_enemy->set_damage(new_enemy->get_damage() + 10 * current_level);
    }

    // tower spawn
    spawnTower();

    std::cout << "=== LEVEL " << current_level << " STARTED ===\n";
    std::cout << "Field size: " << new_size << "x" << new_size << "\n";
}

void game::levelUpMenu()
{
    std::cout << "\n--- LEVEL UP UPGRADE ---\n";
    std::cout << "Choose your reward:\n";
    std::cout << "1. Increase player damage (+10)\n";
    std::cout << "2. Increase spell damage (All spells +10 dmg)\n";
    std::cout << "3. Increase player base health (+100hp)\n";

    int choice = 0;
    while (choice == 0)
    {
        std::string input_str;
        std::cout << "Choose (1-3): ";
        std::cin >> input_str;

        if (input_str == "1")
            choice = 1;
        else if (input_str == "2")
            choice = 2;
        else if (input_str == "3")
            choice = 3;
        else
        {
            std::cout << "Invalid input! Please enter 1, 2 or 3.\n";
        }
    }

    switch (choice)
    {
    case 1:
        player_ptr->set_damage(player_ptr->get_damage() + 10);
        std::cout << "Player damage increased and now equals" << player_ptr->get_damage() << "\n";
        break;
    case 2:
    {
        Hand *hand = player_ptr->get_hand();
        for (int i = 0; i < hand->getSpellCount(); i++)
        {
            hand->getSpell(i)->buffDamage(10);
        }
        std::cout << "All spells upgraded!\n";
        break;
    }
    case 3:
        player_ptr->set_health(player_ptr->get_max_health_points() + 100);
        break;
    }
}

game::game() : field_ptr(nullptr), player_ptr(nullptr), is_game_over(false), next_enemy_id(0), turn(0), gen(rd()), dis2(0, 1), dis1(1, 9), current_level(1){
    field_ptr = new game_field(10, 10);
    player_ptr = new Player();
}

game::~game(){
    cleanup();
}

void game::cleanup(){
    if (player_ptr != nullptr)
    {
        delete player_ptr;
        player_ptr = nullptr;
    }

    if (field_ptr != nullptr)
    {
        delete field_ptr;
        field_ptr = nullptr;
    }

    for (Enemy *enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();

    for (EnemyTower *tower : towers)
    {
        if (tower != nullptr)
        {
            delete tower;
        }
    }
    towers.clear();
}

void game::initializeGame(){
    field_ptr->get_cell(player_ptr->get_position_x(), player_ptr->get_position_y()).setPlayer(true);
    spawnTower();
    spawnEnemy();
    spawnEnemy();
    spawnEnemy();
}

game_field *game::get_field() const{
    return field_ptr;
}

int game::get_turn() const{
    return turn;
}

bool game::isGameOver() const{
    return is_game_over;
}

void game::castSpell(){
    Hand *hand = player_ptr->get_hand();

    if (hand->getSpellCount() == 0)
    {
        std::cout << "You have no spells!" << std::endl;
        return;
    }

    hand->printSpells();
    std::cout << "Choose spell (1-" << hand->getSpellCount() << ") or 0 to cancel: ";

    std::string index_str;
    std::cin >> index_str;

    int spell_index;
    try
    {
        spell_index = std::stoi(index_str);
    }
    catch (...)
    {
        std::cout << "Invalid input! Cancelled.\n";
        return;
    }

    if (spell_index == 0)
    {
        std::cout << "Spell casting cancelled." << std::endl;
        return;
    }

    spell_index--;

    if (spell_index < 0 || spell_index >= hand->getSpellCount())
    {
        std::cout << "Invalid spell index!" << std::endl;
        return;
    }

    Spell *spell = hand->getSpell(spell_index);
    std::cout << "Enter target coordinates (x y): ";
    std::string x_str, y_str;
    std::cin >> x_str >> y_str;

    int target_x, target_y;
    try
    {
        target_x = std::stoi(x_str);
        target_y = std::stoi(y_str);
    }
    catch (...)
    {
        std::cout << "Invalid coordinates! Cancelled.\n";
        return;
    }

    if (!spell->canCast(field_ptr, player_ptr->get_position_x(), player_ptr->get_position_y(), target_x, target_y))
    {
        add_log("Cannot cast spell at this location!"); 
        return;
    }

    bool spell_cast_success = false;

    if (field_ptr->get_cell(target_x, target_y).hasEnemy())
    {
        Enemy *enemy = get_Enemy_at(target_x, target_y);
        if (enemy != nullptr)
        {
            spell->apply(field_ptr, player_ptr->get_position_x(), player_ptr->get_position_y(),
                         target_x, target_y, dynamic_cast<Entity *>(enemy));

            spell_cast_success = true;

            if (enemy->is_dead())
            {
                add_log("Enemy defeated by spell!");
                player_ptr->increment_kills();
                RewardSpell();
                erase_dead_Enemies();
                checkGameWin();
            }
        }
    }
    else if (field_ptr->get_cell(target_x, target_y).hasTower())
    {
        EnemyTower *tower = get_Tower_at(target_x, target_y);
        if (tower != nullptr)
        {
            tower->take_damage(spell->getDamage());
            
            add_log("Tower hit by spell!");

            spell_cast_success = true;

            if (tower->is_destroyed())
            {
                add_log("Tower destroyed!");
            }

            erase_destroyed_towers();
            checkGameWin();
        }
    }
    else 
    {
        spell->apply(field_ptr, player_ptr->get_position_x(), player_ptr->get_position_y(), 
                     target_x, target_y, nullptr);
        
        if (field_ptr->get_cell(target_x, target_y).hasTrap()) {
             spell_cast_success = true; 
        }
    }
    

    FireballSpell* area_spell = dynamic_cast<FireballSpell*>(spell);
    if (area_spell != nullptr) {
        spell_cast_success = true; 

        int area_size = area_spell->getAreaSize();
        for (int dy = 0; dy < area_size; dy++) {
            for (int dx = 0; dx < area_size; dx++) {
                int check_x = target_x + dx;
                int check_y = target_y + dy;
                if (field_ptr->isValidPosition(check_x, check_y) && field_ptr->get_cell(check_x, check_y).hasEnemy()) {
                    Enemy* enemy = get_Enemy_at(check_x, check_y);
                    if (enemy != nullptr) {
                        enemy->take_damage(spell->getDamage());
                        if (enemy->is_dead()) {
                            add_log("Enemy at (" + std::to_string(check_x) + ", " + std::to_string(check_y) + ") defeated by area spell!"); // LOG
                            player_ptr->increment_kills();
                            RewardSpell();
                        }
                    }
                }
            }
        }
        erase_dead_Enemies();
        checkGameWin();
    }

    if (spell_cast_success) {
        add_log("Spell consumed!");
        hand->removeSpell(spell_index);
    } else {
        add_log("Nothing happened. Spell retained.");
    }
}

void game::RewardSpell()
{
    int kills = player_ptr->get_enemies_killed();
    Hand *hand = player_ptr->get_hand();

    if (kills % 2 == 0 && !hand->isFull())
    {
        std::uniform_int_distribution<> spell_dis(0, 2);
        int random_spell = spell_dis(gen);

        Spell *new_spell = nullptr;
        if (random_spell == 0)
        {
            new_spell = new LightningSpell();
        }
        else if (random_spell == 1)
        {
            new_spell = new FireballSpell();
        }
        else
        {
            new_spell = new TrapSpell();
        }

        std::cout << "=== You've earned a new spell! ===" << std::endl;
        hand->addSpell(new_spell);
    }
}

void game::enemy_movement(Enemy *enemy_ptr, int enemy_x, int enemy_y, int player_x, int player_y){
    int random_value = dis2(gen);
    int new_x;
    int new_y;

    if (enemy_x == player_x)
    {
        new_x = enemy_x;
        new_y = (enemy_y > player_y) ? enemy_y - 1 : enemy_y + 1;
    }
    else if (enemy_y == player_y)
    {
        new_y = enemy_y;
        new_x = (enemy_x > player_x) ? enemy_x - 1 : enemy_x + 1;
    }
    else if (random_value == 1)
    {
        new_x = (enemy_x > player_x) ? enemy_x - 1 : enemy_x + 1;
        new_y = enemy_y;
    }
    else
    {
        new_y = (enemy_y > player_y) ? enemy_y - 1 : enemy_y + 1;
        new_x = enemy_x;
    }

    if (!field_ptr->isValidPosition(new_x, new_y))
    {
        return;
    }

    if (field_ptr->get_cell(new_x, new_y).hasEnemy())
    {
        return;
    }

    if (field_ptr->get_cell(new_x, new_y).hasPlayer())
    {
        enemy_deal_damage(enemy_ptr);
        return;
    }

    if (field_ptr->get_cell(new_x, new_y).hasTower())
    {
        return;
    }

    field_ptr->get_cell(enemy_x, enemy_y).setEnemy(false);
    enemy_ptr->set_position(new_x, new_y, field_ptr);
    field_ptr->get_cell(new_x, new_y).setEnemy(true);
}

void game::enemy_deal_damage(Enemy *enemy_ptr)
{
    player_ptr->take_damage(enemy_ptr->get_damage());
    add_log("Enemy attacks you for " + std::to_string(enemy_ptr->get_damage()) + " damage!");
    if (player_ptr->is_dead())
    {
        checkGameOver();
    }
}

void game::spawnEnemy(){
    int x = dis1(gen);
    int y = dis1(gen);
    while (field_ptr->get_cell(x, y).hasEnemy() || field_ptr->get_cell(x, y).hasPlayer() || field_ptr->get_cell(x, y).hasTower())
    {
        x = dis1(gen);
        y = dis1(gen);
    }
    Enemy *enemy = new Enemy(x, y);
    enemies.push_back(enemy);
    field_ptr->get_cell(x, y).setEnemy(true);
}

void game::spawnTower(){
    int x = dis1(gen);
    int y = dis1(gen);
    while (field_ptr->get_cell(x, y).hasEnemy() || field_ptr->get_cell(x, y).hasPlayer() || field_ptr->get_cell(x, y).hasTower())
    {
        x = dis1(gen);
        y = dis1(gen);
    }
    EnemyTower *tower = new EnemyTower(x, y, 15, 2, 2);
    towers.push_back(tower);
    field_ptr->get_cell(x, y).setTower(true);
    std::cout << "Enemy tower spawned at (" << x << ", " << y << ")!" << std::endl;
}

void game::spawnEnemyAt(int x, int y)
{
    if (!field_ptr->isValidPosition(x, y))
    {
        std::cout << "Cannot spawn enemy: invalid position (" << x << ", " << y << ")" << std::endl;
        return;
    }

    if (field_ptr->get_cell(x, y).hasEnemy() ||
        field_ptr->get_cell(x, y).hasPlayer() ||
        field_ptr->get_cell(x, y).hasTower())
    {
        std::cout << "Cannot spawn enemy: position occupied" << std::endl;
        return;
    }

    Enemy *enemy = new Enemy(x, y);
    enemies.push_back(enemy);
    field_ptr->get_cell(x, y).setEnemy(true);
}

void game::spawnTowerAt(int x, int y){
    if (!field_ptr->isValidPosition(x, y))
    {
        std::cout << "Cannot spawn tower: invalid position (" << x << ", " << y << ")" << std::endl;
        return;
    }

    if (field_ptr->get_cell(x, y).hasEnemy() ||
        field_ptr->get_cell(x, y).hasPlayer() ||
        field_ptr->get_cell(x, y).hasTower())
    {
        std::cout << "Cannot spawn tower: position occupied" << std::endl;
        return;
    }

    EnemyTower *tower = new EnemyTower(x, y, 15, 2, 2);
    towers.push_back(tower);
    field_ptr->get_cell(x, y).setTower(true);
}

Enemy *game::get_Enemy_at(int x, int y) const{
    for (Enemy *enemy_ptr : enemies)
    {
        if (enemy_ptr != nullptr && enemy_ptr->get_position_x() == x && enemy_ptr->get_position_y() == y)
        {
            return enemy_ptr;
        }
    }
    return nullptr;
}

EnemyTower *game::get_Tower_at(int x, int y) const{
    for (EnemyTower *tower : towers)
    {
        if (tower != nullptr && tower->getPositionX() == x && tower->getPositionY() == y)
        {
            return tower;
        }
    }
    return nullptr;
}

void game::initializeGameEmpty()
{
    player_ptr = new Player();
    field_ptr->get_cell(player_ptr->get_position_x(), player_ptr->get_position_y()).setPlayer(true);
}

void game::erase_dead_Enemies()
{
    auto i = enemies.begin();
    while (i != enemies.end())
    {
        if ((*i)->is_dead())
        {
            field_ptr->get_cell((*i)->get_position_x(), (*i)->get_position_y()).setEnemy(false);
            delete *i;
            i = enemies.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void game::erase_destroyed_towers(){
    auto i = towers.begin();
    while (i != towers.end())
    {
        if ((*i)->is_destroyed())
        {
            field_ptr->get_cell((*i)->getPositionX(), (*i)->getPositionY()).setTower(false);
            delete *i;
            i = towers.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

bool game::allEnemiesAndTowersDestroyed() const{
    for (Enemy *enemy_ptr : enemies)
    {
        if (enemy_ptr != nullptr && !enemy_ptr->is_dead())
        {
            return false;
        }
    }

    for (EnemyTower *tower : towers)
    {
        if (tower != nullptr && !tower->is_destroyed())
        {
            return false;
        }
    }

    return true;
}

void game::next_turn(){
    turn += 1;
}

void game::tower_turn(){
    for (EnemyTower *tower : towers)
    {
        if (tower->canAttack())
        {
            int tower_x = tower->getPositionX();
            int tower_y = tower->getPositionY();
            int player_x = player_ptr->get_position_x();
            int player_y = player_ptr->get_position_y();

            int distance = std::max(std::abs(tower_x - player_x), std::abs(tower_y - player_y));

            if (distance <= tower->getRadius())
            {
                tower->castTowerSpell(field_ptr, dynamic_cast<Entity *>(player_ptr), tower_x, tower_y, player_x, player_y);

                tower->attack();

                add_log("Tower at (" + std::to_string(tower_x) + ", " + std::to_string(tower_y) + ") attacks you for " + std::to_string(tower->getDamage()) + " damage!");
                if (player_ptr->is_dead())
                {
                    checkGameOver();
                }
            }
        }
        tower->reduceCooldown();
    }
    erase_destroyed_towers();
}

void game::enemy_turn(){
    for (Enemy *enemy_ptr : enemies)
    {
        if (enemy_ptr != nullptr && !enemy_ptr->is_dead())
        {
            int old_x = enemy_ptr->get_position_x();
            int old_y = enemy_ptr->get_position_y();

            enemy_movement(enemy_ptr, old_x, old_y, player_ptr->get_position_x(), player_ptr->get_position_y());

            int new_x = enemy_ptr->get_position_x();
            int new_y = enemy_ptr->get_position_y();

            if (field_ptr->get_cell(new_x, new_y).hasTrap())
            {
                int trap_damage = field_ptr->get_cell(new_x, new_y).getTrapDamage();
                enemy_ptr->take_damage(trap_damage);
                std::cout << "Enemy triggered trap at (" << new_x << ", " << new_y << ") and took " << trap_damage << " damage!" << std::endl;
                field_ptr->get_cell(new_x, new_y).setTrap(false, 0);

                if (enemy_ptr->is_dead())
                {
                    std::cout << "Enemy died from trap!" << std::endl;
                    player_ptr->increment_kills();
                    RewardSpell();
                }
            }
        }
    }
    erase_dead_Enemies();
}

void game::PlayerTurn(int dx, int dy)
{
    message_log.clear();
    int new_x = player_ptr->get_position_x() + dx;
    int new_y = player_ptr->get_position_y() + dy;

    if (!field_ptr->isValidPosition(new_x, new_y))
    {
        add_log("Cannot move outside the field!\n");
        return;
    }

    if (field_ptr->get_cell(new_x, new_y).hasEnemy())
    {
        Enemy *enemy_ptr = get_Enemy_at(new_x, new_y);
        enemy_ptr->take_damage(player_ptr->get_damage());
        add_log("Player attacks enemy for " + std::to_string(player_ptr->get_damage()) + " damage!");
        if (enemy_ptr->is_dead())
        {
            add_log("Enemy defeated!\n");
            player_ptr->increment_kills();
            RewardSpell();
            erase_dead_Enemies();
            checkGameWin();
        }
        return;
    }
    else if (field_ptr->get_cell(new_x, new_y).hasTower())
    {
        add_log("Cannot move there! Tower is in the way!\n");
        return;
    }

    field_ptr->get_cell(player_ptr->get_position_x(), player_ptr->get_position_y()).setPlayer(false);
    player_ptr->set_position(new_x, new_y, field_ptr);
    field_ptr->get_cell(player_ptr->get_position_x(), player_ptr->get_position_y()).setPlayer(true);
    add_log("Player moved to (" + std::to_string(new_x) + ", " + std::to_string(new_y) + ")\n");
}

void game::checkGameOver()
{
    if (player_ptr->get_health_points() <= 0)
    {
        is_game_over = true;
        std::system("clear");
        std::cout << "====================================" << std::endl;
        std::cout << "             YOU LOSE" << std::endl;
        std::cout << "====================================" << std::endl;
    }
}

void game::checkGameWin()
{
    if (allEnemiesAndTowersDestroyed())
    {
        std::cout << "Level cleared!\n";
        nextLevel();
    }
}

void game::print_info()
{
    std::cout << "=== Game Controls ===" << std::endl;
    std::cout << "W - Move up" << std::endl;
    std::cout << "A - Move left" << std::endl;
    std::cout << "S - Move down" << std::endl;
    std::cout << "D - Move right" << std::endl;
    std::cout << "C - Cast spell" << std::endl;
    std::cout << "V - Save game" << std::endl;
    std::cout << "L - Load game" << std::endl;
    std::cout << "H - Skip turn" << std::endl;
    std::cout << "Q - Quit game" << std::endl;
    std::cout << "=====================" << std::endl;
}

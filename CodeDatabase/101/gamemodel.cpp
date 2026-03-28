#include "gamemodel.h"


GameModel::GameModel() :
    enemyTower(new Tower(11, 0)),
    build(new SpawnBuilding(17, 0)),
    mapGame(new Map(*enemyTower, *build, FIELD_HEIGHT, FIELD_WIDTH)),
    user(new Player(3, 19, 3)),
    computer(new Enemy(10, 10)),
    myLeons(),
    myButtons(new Buttons()),
    currentOption(mainMenu),
    currentLevel(1),
    playerLevel(1),
    skillPoints(0),
    levelCompleted(false)
{
    score = 0;
    flagAtack = 0;
    m_inGame = false;
    playerDamageMultiplier = 1.0f;
    spellPowerMultiplier = 1.0f;
    computer->set_alive(true);
    computer->recovery_health();
    enemyTower->set_alive(true);
    enemyTower->recovery_health();
    height = FIELD_HEIGHT;
    width = FIELD_WIDTH;
    cell_height = DOT_HEIGHT;
    cell_width = DOT_WIDTH;
    notifyGameStarted();
}



GameModel::~GameModel(){
    delete myButtons;
    delete build;
    delete mapGame;
    delete user;
    delete computer;
    delete enemyTower;

    for (Leon* leon : myLeons) {
        if (leon != nullptr) {
            delete leon;
        }
    }
    myLeons.clear();
}
int GameModel::get_height(){
    return height;
}

int GameModel::get_width(){
    return width;
}

int GameModel::get_cell_height(){
    return cell_height;
}

int GameModel::get_cell_width(){
    return cell_width;
}

Player &GameModel::get_player()
{
    return *user;
}

Enemy &GameModel::get_enemy()
{
    return *computer;
}

Tower &GameModel::get_tower()
{
    return *enemyTower;
}

Map &GameModel::get_map()
{
    return *mapGame;
}

SpawnBuilding &GameModel::get_build()
{
    return *build;
}

std::vector<Leon*>& GameModel::get_leons()
{
    return myLeons;
}

Buttons &GameModel::get_buttons()
{
    return *myButtons;
}


void GameModel::add_leon(int x, int y)
{
    myLeons.push_back(new Leon(x, y));
    notify();
}

int GameModel::get_score() const{
    return score;
}

std::vector<int> GameModel::check_coords_map(int dx, int dy, int count, int type_character)
{
    int nx, ny;
    int x_slow, y_slow;
    if(type_character == 0){
        nx = get_player().get_x() + dx;
        ny = get_player().get_y() + dy;
        x_slow = get_player().get_x();
        y_slow = get_player().get_y();
    }
    else{
        nx = get_enemy().get_coord_x() + dx;
        ny = get_enemy().get_coord_y() + dy;
        x_slow = get_enemy().get_coord_x();
        y_slow = get_enemy().get_coord_y();
    }
    std::vector<int> ret_info = {0, 0, 0};
    if(nx < 0 || nx >= mapGame->get_height() || ny < 0 || ny >= mapGame->get_width() ) return ret_info;
    else if(mapGame->get_point(nx, ny).get_point_opt() == lock || mapGame->get_point(nx, ny).get_point_opt() == tower) return ret_info;
    else if(mapGame->get_point(x_slow, y_slow).get_point_opt() == slow && count != 3) {
        if(type_character == 0) get_player().set_count_for_slow(1);
        else get_enemy().set_count_for_slow(1);
        return ret_info;
    }
    else if(type_character == 1 && mapGame->get_point(nx, ny).get_point_opt() == trap){
        mapGame->set_point(nx, ny).set_option(unlock);
        ret_info[0] = dx;
        ret_info[1] = dy;
        ret_info[2] = 1;
        return ret_info;
    }
    if(type_character == 0 && count == 3) get_player().set_count_for_slow(-3);
    else if(type_character == 1 && count == 3)get_enemy().set_count_for_slow(-3);
    ret_info[0] = dx;
    ret_info[1] = dy;
    ret_info[2] = 0;
    return ret_info;
}

void GameModel::set_score(int s)
{
    this->score += s;
    notify();
    notifyScoreChanged(this->score);
}


int GameModel::get_flagAtack()
{
    return flagAtack;
}

void GameModel::change_flagAtack(int val)
{
    this->flagAtack = val;
    notify();
}

void GameModel::leon_journey()
{
    if (myLeons.empty()) return;
    const int H = mapGame->get_height();
    const int W = mapGame->get_width();

    std::array<std::pair<int, int>, 4> dirs {{ {1,0}, {-1, 0}, {0, 1}, {0, -1}}};
    std::random_shuffle(dirs.begin(), dirs.end());

    if(std::rand() % 100 < 15) return;
    for(Leon* l : get_leons()){
        if(l != nullptr){
            for (auto [dx, dy] : dirs){
                int nx = l->get_leon_x() + dx;
                int ny = l->get_leon_y() + dy;

                if(nx < 0 || nx >= H || ny < 0 || ny >= W ) continue;
                if(mapGame->get_point(nx, ny).get_point_opt() == lock || mapGame->get_point(nx, ny).get_point_opt() == tower) continue;

                l->set_point_x(nx);
                l->set_point_y(ny);
                hill(l->get_health_ability(), l->get_leon_x(), l->get_leon_y());
                notify();
                break;
            }
        }
    }
    notify();
}

void GameModel::hill(int xp, int x, int y)
{
    if(abs(user->get_x()-x) <= 4 && abs(user->get_y()-y) <= 4 && user->get_health() < 100){
        user->change_health(user->get_health() + xp);
        notifyPlayerHealed(xp);
    }
    notify();
}

void GameModel::player_close_attack()
{
    std::vector<int> result_info;
    result_info = get_player().close_attack();
    int upgradedDamage = get_player().get_damage() * playerDamageMultiplier;
    result_info[2] = upgradedDamage;
    if(abs(get_enemy().get_coord_x() - result_info[0]) <= 1 && abs(get_enemy().get_coord_y() - result_info[1]) <= 1){
        get_enemy().subtract_health(result_info[2]);
        set_score(result_info[2]);
    }

    if(get_enemy().get_health() == 0){
        if(get_tower().get_alive() == true){
            get_enemy().set_alive(false);
            get_build().spawn(get_enemy(), 23, 2);
            get_enemy().set_alive(true);
            get_enemy().recovery_health();
        }
        else{
            get_enemy().set_alive(false);
        }
    }
    notifyPlayerAttacked("Enemy", get_enemy().get_damage(), get_enemy().get_coord_x(), get_enemy().get_coord_y());
}

void GameModel::player_ranged_attack()
{
    std::vector<int> result_info;
    result_info = get_player().ranged_atack();
    int upgradedDamage = get_player().get_damage() * playerDamageMultiplier;
    result_info[2] = upgradedDamage;

    int user_x = result_info[0];
    int user_y = result_info[1];

    int enemy_x = get_enemy().get_coord_x();
    int enemy_y = get_enemy().get_coord_y();

    int tower_x = get_tower().get_left_x();
    int tower_y = get_tower().get_down_y();

    switch(result_info[3]){
    case 0:
        if((user_y + 5) < FIELD_WIDTH && (abs(enemy_x - user_x) <= 1) && (user_y - enemy_y) >= 5 && (user_y - enemy_y) <= 7){
            get_enemy().subtract_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Enemy", result_info[2], enemy_x, enemy_y);
            notifyEnemyDamaged(result_info[2]);
        }
        if((user_y + 5) < FIELD_WIDTH && (user_x - 1) == tower_x && (user_y - 7) == tower_y && get_tower().get_alive() == true){
            get_tower().sub_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Tower", result_info[2], enemy_x, enemy_y);
            notifyTowerDamaged(result_info[2]);
        }
        break;
    case 1:
        if((user_y - 5) >= 0 && (abs(enemy_x - user_x) <= 1) && (enemy_y - user_y) >= 5 && (enemy_y - user_y) <= 7){
            get_enemy().subtract_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Enemy", result_info[2], enemy_x, enemy_y);
            notifyEnemyDamaged(result_info[2]);
        }
        break;
    case 2:
        if((user_x + 5) < FIELD_HEIGHT && (abs(enemy_y - user_y) <= 1) && (enemy_x - user_x) >= 5 && (enemy_x - user_x) <= 7){
            get_enemy().subtract_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Enemy", result_info[2], enemy_x, enemy_y);
            notifyEnemyDamaged(result_info[2]);
        }
        if((user_x + 5) < FIELD_HEIGHT && (user_x + 5) == tower_x && (user_y - 1) == tower_y && get_tower().get_alive() == true){
            get_tower().sub_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Tower", result_info[2], enemy_x, enemy_y);
            notifyTowerDamaged(result_info[2]);
        }
        break;
    case 3:
        if((user_x + 5) >= 0 && (abs(enemy_y - user_y) <= 1) && (user_y - enemy_y) >= 5 && (user_y - enemy_y) <= 7){
            get_enemy().subtract_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Enemy", result_info[2], enemy_x, enemy_y);
            notifyEnemyDamaged(result_info[2]);
        }
        if((user_x + 5) >= 0 && (user_x - 7) == tower_x && (user_y - 1) == tower_y && get_tower().get_alive() == true){
            get_tower().sub_health(result_info[2]);
            set_score(result_info[2]);
            notifyPlayerAttacked("Tower", result_info[2], enemy_x, enemy_y);
            notifyTowerDamaged(result_info[2]);
        }
        break;
    }
    if(get_enemy().get_health() == 0){
        notifyEnemyDied(enemy_x, enemy_y);
        if(get_tower().get_alive() == true){
            get_enemy().set_alive(false);
            get_build().spawn(get_enemy(), 23, 2);
            get_enemy().set_alive(true);
            get_enemy().recovery_health();
        }
        else{
            notifyTowerDestroyed();
            get_enemy().set_alive(false);
        }
    }

}

void GameModel::enemy_journey()
{
    std::vector<int> info_coord = get_enemy().random_journey();
    info_coord = check_coords_map(info_coord[0], info_coord[1], info_coord[2], info_coord[3]);
    if(info_coord[2] == 1){
        get_enemy().subtract_health(get_player().get_hand()->get_trap_card().get_damage());
        get_enemy().set_position(info_coord[0], info_coord[1]);
    }
    else if(info_coord[0] + info_coord[1] != 0){
        int oldX = get_enemy().get_coord_x();
        int oldY = get_enemy().get_coord_y();
        get_enemy().set_position(info_coord[0], info_coord[1]);
        int newX = get_enemy().get_coord_x();
        int newY = get_enemy().get_coord_y();

    }
}

void GameModel::enemy_attack()
{
    change_flagAtack(1);
    std::vector<int> info_attack = get_enemy().enemy_atack();
    int x = info_attack[0];
    int y = info_attack[1];

    int u_x = get_player().get_x();
    int u_y = get_player().get_y();

    if(abs(u_x - x) <= 1 && abs(u_y - y) <= 1){
        get_player().change_health(get_player().get_health() - info_attack[2]);
        notifyEnemyAttacked(info_attack[2]);
        notifyPlayerDamaged(info_attack[2], "Enemy");
    }

}

void GameModel::tower_attack()
{
    std::vector<int> info_attack = get_tower().attack();

    int left_x = info_attack[0];
    int down_y = info_attack[1];

    int user_x = get_player().get_x();
    int user_y = get_player().get_y();

    if(abs(user_x - (left_x + 1)) <= 7 && abs(user_y - (down_y + 1)) <= 7 && get_tower().get_alive()){
        get_player().change_health(get_player().get_health() - get_player().get_hand()->get_card_poison().get_damage()/2);
        notifyPlayerDamaged(get_player().get_hand()->get_card_poison().get_damage()/2, "Tower");
    }
}


void GameModel::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&score), sizeof(score));
    os.write(reinterpret_cast<const char*>(&flagAtack), sizeof(flagAtack));
    os.write(reinterpret_cast<const char*>(&m_inGame), sizeof(m_inGame));
    os.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
    os.write(reinterpret_cast<const char*>(&playerLevel), sizeof(playerLevel));
    os.write(reinterpret_cast<const char*>(&skillPoints), sizeof(skillPoints));
    os.write(reinterpret_cast<const char*>(&levelCompleted), sizeof(levelCompleted));
    os.write(reinterpret_cast<const char*>(&playerDamageMultiplier), sizeof(playerDamageMultiplier));
    os.write(reinterpret_cast<const char*>(&spellPowerMultiplier), sizeof(spellPowerMultiplier));
    int optValue = static_cast<int>(currentOption);
    os.write(reinterpret_cast<const char*>(&optValue), sizeof(optValue));

    enemyTower->serialize(os);
    build->serialize(os);
    mapGame->serialize(os);
    user->serialize(os);
    computer->serialize(os);

    size_t leonsCount = myLeons.size();
    os.write(reinterpret_cast<const char*>(&leonsCount), sizeof(leonsCount));

    for (Leon* leon : myLeons) {
        leon->serialize(os);
    }

    int checksum = score + user->get_health() + playerLevel * 10 + currentLevel * 100;
    os.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи GameModel в поток");
    }
}

void GameModel::deserialize(std::istream& is) {
    // Сохраняем позицию для возврата в случае ошибки
    std::streampos startPos = is.tellg();

    is.read(reinterpret_cast<char*>(&score), sizeof(score));
    is.read(reinterpret_cast<char*>(&flagAtack), sizeof(flagAtack));
    is.read(reinterpret_cast<char*>(&m_inGame), sizeof(m_inGame));
    is.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
    is.read(reinterpret_cast<char*>(&playerLevel), sizeof(playerLevel));
    is.read(reinterpret_cast<char*>(&skillPoints), sizeof(skillPoints));
    is.read(reinterpret_cast<char*>(&levelCompleted), sizeof(levelCompleted));
    is.read(reinterpret_cast<char*>(&playerDamageMultiplier), sizeof(playerDamageMultiplier));
    is.read(reinterpret_cast<char*>(&spellPowerMultiplier), sizeof(spellPowerMultiplier));

    int optValue;
    is.read(reinterpret_cast<char*>(&optValue), sizeof(optValue));
    currentOption = static_cast<OptionGame>(optValue);

    enemyTower->deserialize(is);
    build->deserialize(is);
    mapGame->deserialize(is);
    user->deserialize(is);
    computer->deserialize(is);

    size_t leonsCount;
    is.read(reinterpret_cast<char*>(&leonsCount), sizeof(leonsCount));

    for (Leon* leon : myLeons) {
        delete leon;
    }
    myLeons.clear();

    for (size_t i = 0; i < leonsCount; ++i) {
        Leon* leon = new Leon(1, 1);
        leon->deserialize(is);
        myLeons.push_back(leon);
    }

    int savedChecksum;
    is.read(reinterpret_cast<char*>(&savedChecksum), sizeof(savedChecksum));

    int calculatedChecksum = score + user->get_health() + playerLevel * 10 + currentLevel * 100;

    if (savedChecksum != calculatedChecksum) {
        throw std::runtime_error("Файл сохранения поврежден: несовпадение контрольной суммы");
    }

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения GameModel из потока");
    }
}

OptionGame GameModel::get_current_option() const { return currentOption; }

void GameModel::set_current_option(OptionGame option) {
    currentOption = option;
    notify();
}

int GameModel::get_current_level() const
{
    return currentLevel;
}

void GameModel::set_current_level(int level)
{
    currentLevel = level;
    notifyLevelStarted(level);
}


bool GameModel::get_inGame() {
    return (currentOption == processGame);
}

void GameModel::change_inGame(bool val) {
    m_inGame = val;
    currentOption = val ? processGame : pauseMenu;
    notify();
}


void GameModel::complete_level() {
    levelCompleted = true;
    skillPoints += 2;
    notify();
    notifyLevelCompleted();
}

bool GameModel::is_level_completed() const {
    return levelCompleted;
}


void GameModel::reset_for_new_level() {
    currentLevel++;
    levelCompleted = false;

    computer->set_alive(true);
    computer->recovery_health();

    enemyTower->set_alive(true);
    enemyTower->recovery_health();

    auto hand = user->get_hand();
    int cardsToRemove = hand->get_current_size() / 2;
    Visitor* visitor = new Visitor(this);
    for (int i = 0; i < cardsToRemove; i++) {
        hand->remove_random_card(*visitor);
    }
    delete visitor;

    create_level_map(currentLevel);
    strengthen_enemies(currentLevel);

    notify();
}


void GameModel::create_level_map(int level) {
    height = FIELD_HEIGHT + 5;
    width = FIELD_WIDTH + 5;

    delete mapGame;
    delete enemyTower;
    delete build;

    enemyTower = new Tower(height/2, 0);
    build = new SpawnBuilding(height - 1, 0);
    mapGame = new Map(*enemyTower, *build, height, width);

}


void GameModel::strengthen_enemies(int level) {
    float multiplier = 1.0f + (level - 1) * 0.3f;
    computer->change_health(computer->get_max_health() * multiplier);
    computer->set_damage(computer->get_damage() * multiplier);
    enemyTower->change_health(enemyTower->get_max_health() * multiplier);
}

void GameModel::upgrade_player_health() {
    user->change_health(100);
    user->change_health(user->get_health() + 20);
}

void GameModel::upgrade_player_damage() {
    playerDamageMultiplier += 0.2f;
    user->set_damage(user->get_damage() + playerDamageMultiplier);
}

void GameModel::upgrade_spell_power() {
    spellPowerMultiplier += 0.25f;
}


int GameModel::get_player_level() const {
    return playerLevel;
}

int GameModel::get_skill_points() const {
    return skillPoints;
}

void GameModel::spend_skill_point() {
    if (skillPoints > 0) {
        skillPoints--;
        notify();
    }
}

float GameModel::get_player_damage_multiplier() const { return playerDamageMultiplier; }

float GameModel::get_spell_power_multiplier() const { return spellPowerMultiplier; }

void GameModel::reset_level_completed() {
    levelCompleted = false;
    notify();
}


void GameModel::notifyPlayerHealed(int amount) {
    EventManager::notifyPlayerHealed(amount);
}

void GameModel::notifyPlayerAttacked(const std::string& target, int damage, int x, int y) {
    EventManager::notifyPlayerAttacked(target, damage, x, y);
}

void GameModel::notifyPlayerDamaged(int damage, const std::string& source) {
    EventManager::notifyPlayerDamaged(damage, source);
}

void GameModel::notifyEnemyDamaged(int damage) {
    EventManager::notifyEnemyDamaged(damage);
}

void GameModel::notifyEnemyAttacked(int damage) {
    EventManager::notifyEnemyAttacked(damage);
}

void GameModel::notifyTowerDamaged(int damage) {
    EventManager::notifyTowerDamaged(damage);
}

void GameModel::notifyTowerDestroyed() {
    EventManager::notifyTowerDestroyed();
}

void GameModel::notifyEnemyDied(int x, int y) {
    EventManager::notifyEnemyDied("Enemy", x, y);
}

void GameModel::notifySpellCast(const std::string& spellName, int cost, int x, int y) {
    EventManager::notifySpellCast(spellName, cost, x, y);
}

void GameModel::notifyMoneyChanged(int amount, int newBalance) {
    EventManager::notifyMoneyChanged(amount, newBalance);
}

void GameModel::notifyCardReceived(const std::string& cardName) {
    EventManager::notifyCardReceived(cardName);
}

void GameModel::notifyGameStarted() {
    EventManager::notifyGameStarted();
}

void GameModel::notifyLevelStarted(int level) {
    EventManager::notifyLevelStarted(level);
}

void GameModel::notifyLevelCompleted() {
    EventManager::notifyLevelCompleted();
}

void GameModel::notifyScoreChanged(int newScore) {
    EventManager::notifyScoreChanged(newScore);
}

// gamemodel.cpp
void GameModel::notifyPlayerMoved(int fromX, int fromY, int toX, int toY) {
    EventManager::notifyPlayerMoved(fromX, fromY, toX, toY);
}

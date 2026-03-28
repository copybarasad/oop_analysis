#include "GameField.hpp"
#include "GameContext.hpp"


GameField::GameField(std::unique_ptr<Entity> player, int width = 10, int height = 10, int level = 1) {
    if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
        throw std::range_error("Max size of field is 35");
    }
    if (width < MIN_FIELD_SIZE || height < MIN_FIELD_SIZE) {
        throw std::range_error("Min size of field is 10");
    }
    this->widthField = width;
    this->heightField = height;
    this->gameLevel = level;
    this->gameTurn = 0;
    generateFieldCells(std::move(player));
    generateEnemy();
}


GameField::GameField() {};


void GameField::generateFieldCells(std::unique_ptr<Entity> player) {
    cells.reserve(widthField * heightField);
    for (int i = 0; i < widthField * heightField; ++i) {
        cells.emplace_back(i, i % widthField, i / widthField);
        cells[i].returnCellState().setAvaible(false);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    int randomPoint1, randomPoint2;
    const int minDistance = (widthField + heightField) / 3;
    do {
        randomPoint1 = dist(gen);
        randomPoint2 = dist(gen);
    } while (cells[randomPoint1].getDistance(cells[randomPoint2]) < minDistance);
    FieldCell& cell1 = cells[randomPoint1];
    FieldCell& cell2 = cells[randomPoint2];
    FieldCell& cell3 = [&]() -> FieldCell& {
        const int targetDist = (widthField + heightField) / 4;
        for (auto& cell : cells) {
            int dist1 = cell1.getDistance(cell);
            int dist2 = cell2.getDistance(cell);
            if (dist1 == dist2 && dist1 > targetDist) {
                cell.returnCellState().setAvaible(true);
                return cell;
            }
        }
        int midId = ((cell1.getCoord().second + cell2.getCoord().second) / 2) * widthField +
                    ((cell1.getCoord().first + cell2.getCoord().first) / 2);
        return cells[midId];
    }();
    auto activateAround = [this](const FieldCell& center, int radius) {
        for (auto& cell : cells) {
            if (center.getDistance(cell) < radius) {
                cell.returnCellState().setAvaible(true);
            }
        }
    };
    auto activateLine = [this](const FieldCell& from, const FieldCell& to) {
        auto [x1, y1] = from.getCoord();
        auto [x2, y2] = to.getCoord();
        int dx = x2 - x1, dy = y2 - y1;
        int tolerance = abs(dx) + abs(dy) + widthField + heightField;

        for (auto& cell : cells) {
            auto [x, y] = cell.getCoord();
            if (abs((x - x1) * dy - (y - y1) * dx) <= tolerance) {
                cell.returnCellState().setAvaible(true);
            }
        }
    };
    activateAround(cell1, (widthField + heightField) / 6);
    activateAround(cell2, (widthField + heightField) / 6);
    activateAround(cell3, (widthField + heightField) / 4);
    activateLine(cell1, cell2);
    activateLine(cell1, cell3);
    for (int i = widthField; i < widthField * heightField - widthField; ++i) {
        int up = i - widthField;
        int down = i + widthField;
        int left = i - 1;
        int right = i + 1;
        bool currentCellAvaible = cells[i].returnCellState().getAvaible();
        bool upCellAvaible = cells[up].returnCellState().getAvaible();
        bool downCellAvaible = cells[down].returnCellState().getAvaible();
        bool leftCellAvaible = cells[left].returnCellState().getAvaible();
        bool rightCellAvaible = cells[right].returnCellState().getAvaible();
        if (
            currentCellAvaible &&
            (!upCellAvaible || !downCellAvaible || !leftCellAvaible || !rightCellAvaible) &&
            (i % widthField)
        ) {
            std::shared_ptr<IState> decelerationCellEffect = std::make_shared<DecelerationEffect>();
            cells[i].returnCellState().setConstState(move(decelerationCellEffect));
        }
    }
    spawnEntity(std::make_unique<EnemyTower>(gameLevel), cell3.getIndex());
    spawnEntity(std::make_unique<EnemyBarracks>(gameLevel), randomPoint2);
    spawnEntity(std::move(player), randomPoint1);
}


void GameField::moveEntity(int oldIndex, int newIndex) {
    entityManager.changeEntityIndex(oldIndex, newIndex);
    cells[oldIndex].returnCellState().setAvaible(true);
    cells[newIndex].returnCellState().setAvaible(false);
}


bool GameField::isMoveCorrect(int oldIndex, int newIndex) {
    if (oldIndex < 0 || oldIndex >= widthField * heightField ||
        newIndex < 0 || newIndex >= widthField * heightField) {
        return false;
    }
    if (!cells[newIndex].returnCellState().getAvaible()) {
        return false;
    }
    int oldX = oldIndex % widthField;
    int newX = newIndex % widthField;
    if (abs(oldX - newX) > 1) {
        return false;
    }
    return true;
}


void GameField::spawnEntity(std::unique_ptr<Entity> entity, int index) {
    cells[index].returnCellState().setAvaible(false);
    entityManager.createEntity(std::move(entity), index);
}


void GameField::generateEnemy() {
    int countEnemy = sqrt(((widthField + heightField) / 2) + gameLevel);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    do {
        int randomIndex = dist(gen);
        bool randomCellAvailable = cells[randomIndex].returnCellState().getAvaible();
        float distanceToPlayer = cells[playerIndex].getDistance(cells[randomIndex]);
        if (randomCellAvailable && (distanceToPlayer > ((widthField + heightField) / 3))) {
            std::unique_ptr<Entity> enemy = std::make_unique<Enemy>(gameLevel);
            spawnEntity(std::move(enemy), randomIndex);
            --countEnemy;
        }
    } while (countEnemy >= 0);
}


bool GameField::enemyOnIndex(int index) const {
    if (entityManager[index]) {
        auto type = entityManager[index]->getType();
        if (type == Entity::entityType::ENEMY ||
            type == Entity::entityType::BARRACKS ||
            type == Entity::entityType::TOWER) {
                return true;
        }
    }
    return false;
}


int GameField::firstEnemyIndexOnLine(int oldIndex, int newIndex) {
    int totalCells = widthField * heightField;
    int rowOld = oldIndex / widthField;
    int rowNew = newIndex / widthField;
    bool isHorizontal = (rowOld == rowNew);

    if (isHorizontal) {
        int dir = (newIndex > oldIndex) ? 1 : -1;
        int current = oldIndex + dir;
        while (current >= 0 && current < totalCells && (current / widthField) == rowOld) {
            if (!cells[current].returnCellState().getAvaible() && !enemyOnIndex(current)) {
                return -1;
            }
            if (enemyOnIndex(current)) {
                return current;
            }
            current += dir;
        }
    } else {
        int dir = (newIndex > oldIndex) ? widthField : -widthField;
        int current = oldIndex + dir;
        while (current >= 0 && current < totalCells) {
            if (!cells[current].returnCellState().getAvaible() && !enemyOnIndex(current)) {
                return -1;
            }
            if (enemyOnIndex(current)) {
                return current;
            }
            current += dir;
        }
    }
    return -1;
}


bool GameField::playerTurn(char command) {
    gameTurn++;
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    GameContext ctx{cells, entityManager};
    int newPlayerIndex;
    int enemyIndex;
    int powerOfSpells;
    bool move = false;
    bool cellSlowed = entityManager[playerIndex]->checkDebaffState();
    switch (command) {
    case 'w':
        if (cellSlowed) {
            return true;
        }
        newPlayerIndex = playerIndex - widthField;
        move = true;
        break;
    case 'a':
        if (cellSlowed) {
            return true;
        }
        newPlayerIndex = playerIndex - 1;
        move = true;
        break;
    case 's':
        if (cellSlowed) {
            return true;
        }
        newPlayerIndex = playerIndex + widthField;
        move = true;
        break;
    case 'd':
        if (cellSlowed) {
            return true;
        }
        newPlayerIndex = playerIndex + 1;
        move = true;
        break;
    case 'e':
        powerOfSpells = entityManager[playerIndex]->getPowerOfSpell();
        entityManager[playerIndex]->useItem(ctx, playerIndex, powerOfSpells);
        return true;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
        entityManager[playerIndex]->swapItemInHand(command-'0');
        break;
    case 'q':
        entityManager[playerIndex]->swapWeapon();
        return true;
    default:
        break;
    }
    if (move) {
        int playerDamage = entityManager[playerIndex]->getDamage();
        if (entityManager[newPlayerIndex]) {
            animateSwordAttack(newPlayerIndex);
            entityManager[newPlayerIndex]->causeDamage(playerDamage);
        }
        else if (!entityManager[playerIndex]->melle()) {
            enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
            if (enemyIndex != -1) {
                animateBowAttack(playerIndex, enemyIndex);
                entityManager[enemyIndex]->causeDamage(playerDamage);
            }
            else {
                if (isMoveCorrect(playerIndex, newPlayerIndex)) {
                    if (cells[newPlayerIndex].returnCellState().haveSpecificState()) {
                        entityManager[playerIndex]->setDebaffState(); // to do: сделать этот метод под IState;
                    }
                    moveEntity(playerIndex, newPlayerIndex);
                }
            }
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            if (cells[newPlayerIndex].returnCellState().haveSpecificState()) {
                entityManager[playerIndex]->setDebaffState(); // to do: сделать этот метод под IState;
            }
            moveEntity(playerIndex, newPlayerIndex);
        }
    }
    return move;
}


int GameField::getCountOfEnemy() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    return enemyIndexes.size() + barrackIndexes.size() + towerIndexes.size();
}


void GameField::playerLevelUp(char attribute) {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    int prevInt = entityManager[playerIndex]->getInt();
    int prevStr = entityManager[playerIndex]->getStr();
    int prevDex = entityManager[playerIndex]->getDex();
    switch (attribute) {
    case '1':
        entityManager[playerIndex]->setInt(prevInt + 10);
        entityManager[playerIndex]->addSpells(0, 10);
        entityManager[playerIndex]->addSpells(1, 10);
        entityManager[playerIndex]->addSpells(2, 2);
        entityManager[playerIndex]->addSpells(3, 1);
        break;
    case '2':
        entityManager[playerIndex]->setStr(prevStr + 10);
        break;
    case '3':
        entityManager[playerIndex]->setDex(prevDex + 10);
        break;
    default:
        break;
    }
}


void GameField::update() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    for (int index : enemyIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].returnCellState().setAvaible(true);
            std::shared_ptr<IState> coupreAnimate = std::make_shared<CoupreAnimate>();
            cells[index].returnCellState().setTemporaryState(move(coupreAnimate));
            entityManager[playerIndex]->addExperience(enemyLevel * 10 + 10);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    for (int index : barrackIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].returnCellState().setAvaible(true);
            std::shared_ptr<IState> coupreAnimate = std::make_shared<CoupreAnimate>();
            cells[index].returnCellState().setTemporaryState(move(coupreAnimate));
            entityManager[playerIndex]->addExperience(enemyLevel * 30 + 10);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    for (int index : towerIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].returnCellState().setAvaible(true);
            std::shared_ptr<IState> coupreAnimate = std::make_shared<CoupreAnimate>();
            cells[index].returnCellState().setTemporaryState(move(coupreAnimate));
            entityManager[playerIndex]->addExperience(enemyLevel * 50 + 10);
            entityManager.killEntity(index);
        }
    }
}


void GameField::enemyTurn() {
    std::unordered_map<int, int> grid;
    grid.reserve(widthField * heightField);
    auto enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    for (int i = 0; i < widthField*heightField; ++i) {
        if (cells[i].returnCellState().getAvaible() ||
            std::find(enemyIndexes.begin(), enemyIndexes.end(), i) != enemyIndexes.end() ||
            i == playerIndex
            ) {
            grid[i] = 0;
        }
        else {
            grid[i] = -1;
        }
    }
    std::queue<int> wave;
    wave.push(playerIndex);
    while (!wave.empty()) {
        int currentIndex = wave.front();
        int waveCount = grid[currentIndex];
        wave.pop();
        int upIndex = currentIndex - widthField;
        int downIndex = currentIndex + widthField;
        int leftIndex = currentIndex - 1;
        int rightIndex = currentIndex + 1;
        if (upIndex >= 0 && upIndex < widthField * heightField) {
            auto it = grid.find(upIndex);
            if (it != grid.end() && it->second == 0) {
                if (isMoveCorrect(currentIndex, upIndex)) {
                    grid[upIndex] = waveCount + 1;
                    wave.push(upIndex);
                }
            }
        }
        if (downIndex >= 0 && downIndex < widthField * heightField) {
            auto it = grid.find(downIndex);
            if (it != grid.end() && it->second == 0) {
                if (isMoveCorrect(currentIndex, downIndex)) {
                    grid[downIndex] = waveCount + 1;
                    wave.push(downIndex);
                }
            }
        }
        if (leftIndex >= 0 && leftIndex < widthField * heightField) {
            auto it = grid.find(leftIndex);
            if (it != grid.end() && it->second == 0) {
                if (isMoveCorrect(currentIndex, leftIndex)) {
                    grid[leftIndex] = waveCount + 1;
                    wave.push(leftIndex);
                }
            }
        }
        if (rightIndex >= 0 && rightIndex < widthField * heightField) {
            auto it = grid.find(rightIndex);
            if (it != grid.end() && it->second == 0) {
                if (isMoveCorrect(currentIndex, rightIndex)) {
                    grid[rightIndex] = waveCount + 1;
                    wave.push(rightIndex);
                }
            }
        }
    }
    std::vector<std::pair<int,int>> enemyIndexesWithDistance;
    for(auto enemyIndex: enemyIndexes) {
        enemyIndexesWithDistance.push_back({enemyIndex, grid[enemyIndex]});
    }
    std::sort(enemyIndexesWithDistance.begin(),
                enemyIndexesWithDistance.end(),
                [](const auto& a, const auto& b) {return a.second < b.second;}
            );
    for (auto [index, distance] : enemyIndexesWithDistance) {
        Entity* currentEnemy = entityManager[index];
        if (!currentEnemy) continue;
        if (!currentEnemy->alive()) continue;
        if (cells[playerIndex].getDistance(cells[index]) == 1.0) {
            Entity* player = entityManager[playerIndex];
            if (player) {
                player->causeDamage(currentEnemy->getDamage());
            }
            continue;
        }
        int currentEnemyBestTurn = getBestTurnForEnemy(index, playerIndex, grid);
        bool haveSpecificState = cells[currentEnemyBestTurn].returnCellState().haveSpecificState();

        if (haveSpecificState) {
            int trapDamage = cells[currentEnemyBestTurn].returnCellState().getStateDamage();
            currentEnemy->causeDamage(trapDamage);
        }
        if (currentEnemyBestTurn == index) continue;
        if (isMoveCorrect(index, currentEnemyBestTurn)) {
            if (!entityManager[currentEnemyBestTurn] && cells[currentEnemyBestTurn].returnCellState().getAvaible()) {
                moveEntity(index, currentEnemyBestTurn);
            }
        }
    }
}


int GameField::getBestTurnForEnemy(int enemyIndex, int playerIndex, std::unordered_map<int, int>& grid) {
    std::vector<int> directions = {
        enemyIndex - widthField,
        enemyIndex + widthField,
        enemyIndex - 1,
        enemyIndex + 1
    };

    int bestTurn = enemyIndex;
    int bestDistance = std::numeric_limits<int>::max();

    for (int turn : directions) {
        if (turn < 0 || turn >= widthField * heightField) continue;
        auto it = grid.find(turn);
        if (it == grid.end() || it->second <= 0) continue;
        if (!isMoveCorrect(enemyIndex, turn)) continue;
        if (it->second < bestDistance) {
            bestDistance = it->second;
            bestTurn = turn;
        }
        else if (it->second == bestDistance) {
            float currentDist = cells[bestTurn].getDistance(cells[playerIndex]);
            float newDist = cells[turn].getDistance(cells[playerIndex]);
            if (newDist < currentDist) {
                bestTurn = turn;
            }
        }
    }
    return bestTurn;
}


std::vector<std::pair<int, float>>
GameField::getDistanceToPlayer(std::vector<int> enemyIndexes, int playerIndex) {
    std::vector<std::pair<int, float>> distances;
    float distance;
    for (long unsigned int i = 0; i < enemyIndexes.size(); ++i) {
        distance = cells[playerIndex].getDistance(cells[enemyIndexes[i]]);
        distances.push_back({enemyIndexes[i], distance});
    }
    return distances;
}


void GameField::buildingsTurn() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    int index;
    if (!barrackIndexes.empty()) {
        index = barrackIndexes[0];
        if (entityManager[index]->timeToSpawn()) {
            int up = index - widthField;
            int down = index + widthField;
            int left = index - 1;
            int right = index + 1;

            if (isMoveCorrect(index, up)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), up);
            }
            else if (isMoveCorrect(index, down)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), down);
            }
            else if (isMoveCorrect(index, left)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), left);
            }
            else if (isMoveCorrect(index, right)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), right);
            }
        }
    }
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    int towerIndex;
    GameContext ctx{cells, entityManager};
    if (!towerIndexes.empty()) {
        towerIndex = towerIndexes[0];
        EnemyTower* tower = dynamic_cast<EnemyTower*>(entityManager[towerIndex]);
        if (cells[playerIndex].getDistance(cells[towerIndex]) <= tower->getSpellDistance()) {
            tower->towerAttack(ctx, towerIndex, 0);
        }
    }
}


std::unique_ptr<Entity> GameField::returnPlayer() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    return entityManager.returnEntity(playerIndex);
}


bool GameField::playerAlive() const {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    return entityManager[playerIndex]->alive();
}


std::shared_ptr<PlayerData> GameField::getPlayerData() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    Player* player = dynamic_cast<Player*>(entityManager[playerIndex]);

    std::vector<long long> playerExperience = player->getExperience();
    int playerLevel = player->getLevel();
    bool playerLevelIncreased = player->isLevelIncreased();
    std::pair<int, int> playerHealth = player->getHealth();
    int playerAttack = player->getDamage();
    int playerIntelligence = player->getInt();
    int playerDexterity = player->getDex();
    int playerStrength = player->getStr();
    std::pair<int, int> playerHandSize = player->getHandSize();
    std::vector<std::pair<bool, int>> playerHandInfo = player->getHandItems();
    std::string playerWeapon;
    std::string playerDebaff;
    if (player->melle()) {
        playerWeapon = "Sword";
    }
    else {
        playerWeapon = "Bow";
    }
    if (player->checkDebaffState()) {
        player->setDebaffState();
        playerDebaff = "Slowed";
    }
    PlayerData* data = new PlayerData{
        playerExperience[0],
        playerExperience[1],
        playerExperience[2],
        playerLevel,
        playerLevelIncreased,
        playerHealth.second,
        playerHealth.first,
        playerAttack,
        playerIntelligence,
        playerDexterity,
        playerStrength,
        playerHandSize.first,
        playerHandSize.second,
        playerHandInfo,
        playerWeapon,
        playerDebaff
    };
    std::shared_ptr<PlayerData> dataSharedPtr(data);
    return dataSharedPtr;
}


std::vector<EnemyData> GameField::getEnemyData() {
    std::vector<int> playerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER);
    int playerIndex = playerIndexes[0];
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<EnemyData> data;
    if (!enemyIndexes.empty()) {
        auto enemyIndexesWithDistances = getDistanceToPlayer(enemyIndexes, playerIndex);
        std::sort(enemyIndexesWithDistances.begin(), enemyIndexesWithDistances.end(),
                [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                    return a.second < b.second;
                });
        data.reserve(enemyIndexesWithDistances.size());

        for (const auto& [index, dist] : enemyIndexesWithDistances) {
            Entity* ent = entityManager[index];
            Enemy* enemy = dynamic_cast<Enemy*>(ent);
            if (!enemy) continue;
            EnemyData enemyData{};
            enemyData.enemyAttack = enemy->getDamage();
            std::pair<int, int> enemyHealth = enemy->getHealth();
            enemyData.enemyHealth = enemyHealth.first;
            enemyData.enemyMaxHealth = enemyHealth.second;
            enemyData.name = "Enemy";
            data.push_back(enemyData);
        }
    }
    {
        std::vector<int> barracksIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
        if (!barracksIndexes.empty()) {
            int barracksIndex = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS)[0];
            Entity* ent = entityManager[barracksIndex];
            EnemyBarracks* baracks = dynamic_cast<EnemyBarracks*>(ent);
            EnemyData barracksData{};
            barracksData.enemyAttack = baracks->getDamage();
            std::pair<int, int> baracksHealth = baracks->getHealth();
            barracksData.enemyHealth = baracksHealth.first;
            barracksData.enemyMaxHealth = baracksHealth.second;
            barracksData.name = "Barrack";
            data.push_back(barracksData);
        }
    }
    {
        std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
        if (!towerIndexes.empty()) {
            int towerIndex = entityManager.getIndexesWithEntity(Entity::entityType::TOWER)[0];
            Entity* ent = entityManager[towerIndex];
            EnemyTower* tower = dynamic_cast<EnemyTower*>(ent);
            EnemyData towerData{};
            towerData.enemyAttack = tower->getDamage();
            std::pair<int, int> towerHealth = tower->getHealth();
            towerData.enemyHealth = towerHealth.first;
            towerData.enemyMaxHealth = towerHealth.second;
            towerData.name = "Tower";
            data.push_back(towerData);
        }
    }
    return data;
}


void GameField::animateBowAttack(int playerIndex, int enemyIndex) {
    int animateDelay = 0;
    int currentIndex = playerIndex;
    int step = 0;
    int playerX = playerIndex % GlobalGameConfig::fieldWidth;
    int playerY = playerIndex / GlobalGameConfig::fieldWidth;
    int enemyX = enemyIndex % GlobalGameConfig::fieldWidth;
    int enemyY = enemyIndex / GlobalGameConfig::fieldWidth;
    char arrowSymbol = '_';
    if (playerX < enemyX && playerY == enemyY) {
        step = 1;
        arrowSymbol = '-';
    }
    else if (playerX > enemyX && playerY == enemyY) {
        step = -1;
        arrowSymbol = '-';
    }
    else if (playerY < enemyY && playerX == enemyX) {
        step = GlobalGameConfig::fieldWidth;
        arrowSymbol = '|';
    }
    else if (playerY > enemyY && playerX == enemyX) {
        step = -GlobalGameConfig::fieldWidth;
        arrowSymbol = '|';
    }
    while (currentIndex != enemyIndex) {
        std::shared_ptr<IState> bowAnimate = std::make_shared<AttackEffect>(arrowSymbol, animateDelay++, 1);
        cells[currentIndex].returnCellState().setTemporaryState(std::move(bowAnimate));
        currentIndex += step;
    }
    std::shared_ptr<IState> hitEffect = std::make_shared<AttackEffect>('*', animateDelay, 1);
    cells[enemyIndex].returnCellState().setTemporaryState(std::move(hitEffect));
}


void GameField::animateSwordAttack(int enemyIndex) {
    std::shared_ptr<IState> swordAnimate = std::make_shared<AttackEffect>();
    cells[enemyIndex].returnCellState().setTemporaryState(move(swordAnimate));
}


std::vector<wchar_t> GameField::show() {
    std::vector<wchar_t> data;
    for (auto& cell: cells) {
        data.push_back((wchar_t)cell.getCellSymbol());
    }
    const std::unordered_map<int, std::unique_ptr<Entity>>& entityMap = entityManager.returnInfoMap();
    for (const auto& [index, entity] : entityMap) {
        if (data[index] != '/') {
            data[index] = (wchar_t)entity->returnEntitySymbol();
        }
    }
    return data;
}


FieldSaveData GameField::getFieldSaveData() {
    FieldSaveData data{};
    EntitySaveData entityData = entityManager.getEntitySaveData();
    std::vector<struct CellSaveData> cellsData;
    for (auto& cell : cells) {
        CellSaveData cellData = cell.getCellSaveData();
        cellsData.push_back(cellData);
    }
    data.cellsData = cellsData;
    data.entityData = entityData;
    data.gameLevel = this->gameLevel;
    data.gameTurn = this->gameTurn;
    data.heightField = this->heightField;
    data.widthField = this->widthField;
    return data;
}


void GameField::setFieldSaveData(FieldSaveData data) {
    this->widthField = data.widthField;
    this->heightField = data.heightField;
    this->gameLevel = data.gameLevel;
    this->gameTurn = data.gameTurn;
    entityManager.setEntitySaveData(data.entityData);
    cells.reserve(this->widthField * this->heightField);
    for (int i = 0; i < (int)cells.size(); ++i) {
        cells[i].setCellSaveData(data.cellsData[i]);
    }
}
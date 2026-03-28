#include "EntityManager.h"

void EntityManager::initEnemies(Field& field, int count, int curLvl) {
    enemies.clear();
    enemies.reserve(count);

    std::vector<std::pair<int,int>> freeCells;

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.canMoveTo(x, y)) {
                freeCells.emplace_back(x, y);
            }
        }
    }

    if (freeCells.empty()) {
        std::cout << "Нет свободного места для спавна врагов!\n";
        return;
    }

    // если врагов больше, чем свободных клеток — спавним максимум
    if ((int)freeCells.size() < count) {
        std::cout << "Предупреждение: недостаточно места. "
                     "Будет заспавнено только "
                  << freeCells.size() << " врагов.\n";
        count = freeCells.size();
    }

    // перемешиваем список доступных клеток
    std::shuffle(freeCells.begin(), freeCells.end(), 
                 std::mt19937(std::random_device{}()));

    for (int i = 0; i < count; ++i) {
        auto [x, y] = freeCells[i];
        enemies.emplace_back(10 * pow(1.2, curLvl - 1), 2, x, y);
        field.setCellEntity(&enemies.back(), x, y);
    }
}

int EntityManager::moveEnemies(Field& field, Player& player){
    for (auto& enemy: enemies){
        if (!enemy.isAlive()){
            continue;
        }

        int ex = enemy.getX();
        int ey = enemy.getY();
        int px = player.getX();
        int py = player.getY();

        int dx = 0, dy = 0;

        if (abs(ex-px)>=abs(ey-py)){
            dx = (px>ex) ? 1 : ((px<ex) ? -1 : 0);
        }  else{
            dy = (py>ey) ? 1 : ((py<ey) ? -1 : 0);
        }

        int newX = ex + dx;
        int newY = ey + dy;

        if (field.canMoveTo(newX, newY)) {
            field.setCellEntity(&enemy, newX, newY);
            field.clearCellEnt(ex, ey);
            enemy.setPosition(newX, newY);
        } else if (field.getCellEntity(newX, newY)!=nullptr && field.getCellEntity(newX, newY)->getObjType() == GameEntType::Player){
            player.takeDamage(enemy.getMeleeDamage());
            if (!player.isAlive()){
                return 1;
            }
        }else {

            std::vector<std::pair<int,int>> dirs = {
                {1,0}, {-1,0}, {0,1}, {0,-1}
            };

            // Перемешиваем dirs случайным образом
            std::shuffle(dirs.begin(), dirs.end(), std::mt19937(std::random_device{}()));

            for (auto [dx2, dy2] : dirs) {
                int nx = ex + dx2;
                int ny = ey + dy2;

                if (field.canMoveTo(nx, ny)) {

                    field.setCellEntity(&enemy, nx, ny);
                    field.clearCellEnt(ex, ey);
                    enemy.setPosition(nx, ny);
                    
                    break;
                }
            }
        }

    }
    return 0;
}

void EntityManager::initTowers(Field& field, int count) {
    towers.clear();
    towers.reserve(count);

    std::vector<std::pair<int,int>> freeCells;

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.canMoveTo(x, y)) {
                freeCells.emplace_back(x, y);
            }
        }
    }

    if (freeCells.empty()) {
        std::cout << "Нет свободного места для спавна башен!\n";
        return;
    }

    if ((int)freeCells.size() < count) {
        std::cout << "Предупреждение: недостаточно места. "
                  << "Будет заспавнено только "
                  << freeCells.size() << " башен.\n";
        count = freeCells.size();
    }

    std::shuffle(freeCells.begin(), freeCells.end(), 
                 std::mt19937(std::random_device{}()));

    for (int i = 0; i < count; ++i) {
        auto [x, y] = freeCells[i];
        towers.emplace_back(x, y, 3.0, 1, 1);
        field.setCellEntity(&towers.back(), x, y);
    }
}

int EntityManager::countAlive() const {
    int alive = 0;
    for (auto& e : enemies)
        if (e.isAlive()) alive++;
    return alive;
}


void EntityManager::updateTowers(Field& field, Player& player) {
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.tryAttack(field, player);
            if (!player.isAlive()) {
                std::cout << "Игрок погиб от атаки башни!\n";
                break;
            }
        }
    }
}



void EntityManager::save(std::ostream& out) const {
    out << enemies.size() << '\n';
    for (const auto& enemy : enemies) {
        out << enemy.getHp() << ' '
            << enemy.getMeleeDamage() << ' '
            << enemy.getX() << ' '
            << enemy.getY() << '\n';
    }

    out << towers.size() << '\n';
    for (const auto& tower : towers) {
        out << tower.getHp() << ' '
            << tower.getX() << ' '
            << tower.getY() << '\n';
    }
}

void EntityManager::load(std::istream& in, Field& field) {
    enemies.clear();
    towers.clear();

    int enemyCount = 0;
    in >> enemyCount;
    if (!in || enemyCount < 0) {
        throw InvalidSaveDataException("Ошибка чтения количества врагов");
    }

    for (int i = 0; i < enemyCount; ++i) {
        double hp = 0;
        double dmg = 0;
        int x = 0, y = 0;
        in >> hp >> dmg >> x >> y;
        if (!in) {
            throw InvalidSaveDataException("Ошибка чтения параметров врага");
        }
        enemies.emplace_back(hp, dmg, x, y);
        if (hp > 0.0) {
            field.setCellEntity(&enemies.back(), x, y);
        }
    }

    int towerCount = 0;
    in >> towerCount;
    if (!in || towerCount < 0) {
        throw InvalidSaveDataException("Ошибка чтения количества башен");
    }

    for (int i = 0; i < towerCount; ++i) {
        double hp = 0;
        int x = 0, y = 0;
        in >> hp >> x >> y;
        if (!in) {
            throw InvalidSaveDataException("Ошибка чтения параметров башни");
        }
        
        towers.emplace_back(x, y, 3.0, 1, 1); // параметры, как при создании
        towers.back().setHealth(hp);
        field.setCellEntity(&towers.back(), x, y);
    }
}
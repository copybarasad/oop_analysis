#include "managment/saveLoadManager.h"

SaveLoadManager::SaveLoadManager(){}

void SaveLoadManager::save(std::string filename, SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int level_)
{
    namespace fs = std::filesystem;

    const auto &allies_ = spellManager_.getAlly();
    const auto &enemies_ = enemyManager_.getEnemy();
    const auto &enemyHuts_ = enemyManager_.getEnemyHut();
    const auto &towers_ = enemyManager_.getTowers();

    fs::path saveDir = "../save";

    try
    {
        if (!fs::exists(saveDir))
        {
            fs::create_directories(saveDir);
        }

        fs::path filePath = saveDir / (filename + ".txt");
        std::ofstream out(filePath);

        if (!out.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для сохранения: " + filePath.string());
        }

        auto ftime = fs::last_write_time(filePath);
        auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
        std::string timestr = std::string(std::ctime(&cftime));
        timestr.pop_back();
        out << " " << timestr << std::endl;

        out << std::format("GameLvl {}", level_) << std::endl;

        out << std::format("Player {} {} {} {} {} {} {}",
                           player_.getHealth(),
                           player_.getDamage(),
                           player_.getPosition().x,
                           player_.getPosition().y,
                           player_.getWeapon(),
                           player_.getLevel(),
                           player_.getScore())
            << std::endl;

        const auto &spells = player_.getHand()->getSpells();
        for (auto &sp : spells)
        {
            out << std::format("Spell {}", sp->name()) << std::endl;
        }

        auto fieldData = field_.getField();
        for (auto &row : fieldData)
        {
            std::string cells = "Field";
            for (auto &x : row)
            {
                cells += std::format(" {} ", x.string());
            }
            out << cells << std::endl;
        }

        for (auto &en : enemies_)
        {
            out << std::format("Enemy {} {} {} {}", en->getHealth(), en->getDamage(), en->getPosition().x, en->getPosition().y) << std::endl;
        }

        for (auto &enH : enemyHuts_)
        {
            out << std::format("EnemyHut {} {} {}", enH->getHealth(), enH->getPosition().x, enH->getPosition().y) << std::endl;
        }

        for (auto &tw : towers_)
        {
            out << std::format("Tower {} {} {}", tw->getHealth(), tw->getPosition().x, tw->getPosition().y) << std::endl;
        }

        for (auto &al : allies_)
        {
            out << std::format("Ally {} {} {} {}", al->getHealth(), al->getDamage(), al->getPosition().x, al->getPosition().y) << std::endl;
        }

        std::cout << "Сохранение прошло успешно: " << filePath.string() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при сохранении: " << e.what() << std::endl;
    }
}

void SaveLoadManager::load(std::string filePath, SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int &level_)
{
    namespace fs = std::filesystem;

    fs::path filepath = filePath;

    std::ifstream in(filepath);
    if (!in.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return;
    }

    

    std::string time;
    std::getline(in, time);

    auto ftime = fs::last_write_time(filepath);
    auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::string timestr = std::string(std::ctime(&cftime));
    timestr.pop_back(); 
    time.erase(time.begin());

    if (time != timestr)
    {
        std::cerr << "Файл поврежден или некорректная дата: " << filepath << std::endl;
        return;
    }

    int hl, dm, lvl, sc;
    Position pos{-1, -1};
    std::string wp;

    enemyManager_.cleanEnemies();
    spellManager_.cleanAlly();

    std::vector<std::vector<int>> loadField;
    std::vector<std::string> spells;

    std::string type;
    while (in >> type)
    {
        if (type == "Player")
        {
            in >> hl >> dm >> pos.x >> pos.y >> wp >> lvl >> sc;
            player_.setHealth(hl);
            player_.setDamage(dm);
            player_.move(pos);
            player_.setWeapon(wp);
            player_.setLevel(lvl);
            player_.setScore(sc);
        }
        else if (type == "Field")
        {
            std::vector<int> row;
            std::string line;
            std::getline(in, line);
            std::stringstream ss(line);
            int val;
            while (ss >> val)
                row.push_back(val);
            loadField.push_back(row);
        }
        else if (type == "Spell")
        {
            std::string spellName;
            std::getline(in, spellName);
            spellName.erase(spellName.begin());
            spells.push_back(spellName);
        }
        else if (type == "GameLvl")
        {
            in >> lvl;
            level_ = lvl;
        }
        else if (type == "Enemy")
        {
            in >> hl >> dm >> pos.x >> pos.y;
            enemyManager_.addEnemy(std::make_unique<Enemy>(hl, dm, pos));
        }
        else if (type == "EnemyHut")
        {
            in >> hl >> pos.x >> pos.y;
            enemyManager_.addEnemyHut(std::make_unique<EnemyHut>(hl, pos));
        }
        else if (type == "Tower")
        {
            in >> hl >> pos.x >> pos.y;
            enemyManager_.addTower(std::make_unique<Tower>(hl, pos));
        }
        else if (type == "Ally")
        {
            in >> hl >> dm >> pos.x >> pos.y;
            spellManager_.addAlly(std::make_unique<Ally>(hl, dm, pos));
        }
    }

    if (loadField.empty())
    {
        std::cerr << "Данные поля отсутствуют в файле сохранения.\n";
        return;
    }

    field_.buildNewField(loadField);
    player_.getHand()->cleanHand();

    for (auto &sp : spells)
        player_.getHand()->addSpell(sp);

    field_.addPlayer(player_.getPosition());

    for (auto &en : enemyManager_.getEnemy())
        field_.addEnemy(en->getPosition());
    for (auto &enH : enemyManager_.getEnemyHut())
        field_.addEnemyHut(enH->getPosition());
    for (auto &al : spellManager_.getAlly())
        field_.addAlly(al->getPosition());
    for (auto &tw : enemyManager_.getTowers())
        field_.addTower(tw->getPosition());
}

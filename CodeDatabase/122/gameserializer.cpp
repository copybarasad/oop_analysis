#include "gameserializer.h"

void GameSerializer::save(GameModel& model, const std::string& path)
{
    JsonOutputFile file = JsonOutputFile::open(path);

    QJsonObject root;
    root["level"]   = model.getLevel();
    root["player"]  = serializePlayer(model);
    root["enemies"] = serializeEnemies(model);
    root["archers"] = serializeArchers(model);
    root["board"]   = serializeBoard(model);
    root["tower"] = serializeTower(model);
    root["building"] = serializeBuilding(model);

    QJsonDocument doc(root);
    file << doc;
    ////
}

void GameSerializer::load(GameModel& model, const std::string& path)
{
    JsonInputFile file = JsonInputFile::open(path);

    QJsonDocument doc;
    file >> doc;

    QJsonObject root = doc.object();

    if (!root.contains("level"))
        throw InvalidDataException("missing 'level'");

    int level = root["level"].toInt();
    model.getLevel() = level;

    model.getPlayers().clear();
    model.getEnemies().clear();
    model.getArchers().clear();

    if (!root.contains("board"))
        throw InvalidDataException("missing 'GameBoard'");
    deserializeBoard(model, root["board"].toObject());

    if (!root.contains("player"))
        throw InvalidDataException("missing 'Player'");
    deserializePlayer(model, root["player"].toObject());

    if (!root.contains("building"))
        throw InvalidDataException("missing 'DarkBuilding'");
    deserializeBuilding(model, root["building"].toObject());

    if (root.contains("tower"))
        deserializeTower(model, root["tower"].toObject());

    if (root.contains("enemies"))
        deserializeEnemies(model, root["enemies"].toArray());

    if (root.contains("archers"))
        deserializeArchers(model, root["archers"].toArray());

    model.isLogicActive() = false;
    model.inLevel() = true;
}


QJsonObject GameSerializer::serializePlayer(GameModel& model) const
{
    auto& player = model.getPlayers().front();
    auto& hands = player->getHands();

    QJsonObject j;
    j["health"] = player->getHealth();
    j["mana"]   = hands.getMana();
    j["x"]      = player->getXPos();
    j["y"]      = player->getYPos();
    j["inUpgradeMode"] = hands.inUpgradeMode();
    j["inTargetMode"] = hands.inTargetMode();
    j["selectedSpell"] = static_cast<int>(hands.getSelectedSpell());

    QJsonObject cards;

    auto addCard = [&](CardType type, const char* name)
    {
        QJsonObject cardObj;
        cardObj["count"]   = hands.getCountCards(type);
        cardObj["upgrade"] = hands.getCard(type)->getUpgradeLevel();
        cards[name] = cardObj;
    };

    addCard(CardType::FIREBALL,      "FIREBALL");
    addCard(CardType::EARTHQUAKE,    "EARTHQUAKE");
    addCard(CardType::POISON_TRAP,   "POISON_TRAP");
    addCard(CardType::SUMMON_ARCHER, "SUMMON_ARCHER");
    addCard(CardType::UPGRADE_CARD,  "UPGRADE_CARD");

    j["cards"] = cards;

    return j;
}

void GameSerializer::deserializePlayer(GameModel& model, const QJsonObject& j)
{
    model.getPlayers().clear();

    int health = j["health"].toInt();
    int mana   = j["mana"].toInt();
    int xPos      = j["x"].toInt();
    int yPos      = j["y"].toInt();

    model.spawnPlayer(health, mana, xPos, yPos);
    auto& hands = model.getPlayers().front()->getHands();
    hands.setUpgradeMode(j["inUpgradeMode"].toBool());
    hands.setTargetMode(j["inTargetMode"].toBool());
    hands.setSelectedSpell(static_cast<CardType>(j["selectedSpell"].toInt()));

    if (j.contains("cards"))
    {
        auto cards = j["cards"].toObject();

        auto restoreCard = [&](CardType type, const char* name)
        {
            if (!cards.contains(name)) return;

            QJsonObject cardObj = cards[name].toObject();

            int count   = cardObj["count"].toInt();
            int upgradeLevel = cardObj["upgrade"].toInt();

            hands.getCountCards(type) = count;
            hands.getCard(type)->clearUpgrades();
            hands.getCard(type)->upgrade(upgradeLevel);
        };

        restoreCard(CardType::FIREBALL,      "FIREBALL");
        restoreCard(CardType::EARTHQUAKE,    "EARTHQUAKE");
        restoreCard(CardType::POISON_TRAP,   "POISON_TRAP");
        restoreCard(CardType::SUMMON_ARCHER, "SUMMON_ARCHER");
        restoreCard(CardType::UPGRADE_CARD,  "UPGRADE_CARD");

        hands.recalcTotalCount();
    }
}

QJsonArray GameSerializer::serializeEnemies(GameModel& model) const
{
    QJsonArray arr;
    for (auto& e : model.getEnemies()) {
        QJsonObject obj;
        obj["health"] = e->getHealth();
        obj["x"] = e->getXPos();
        obj["y"] = e->getYPos();
        obj["damage"] = e->getDamage();
        arr.append(obj);
    }
    return arr;
}

void GameSerializer::deserializeEnemies(GameModel& model, const QJsonArray& j)
{
    for (auto el : j) {
        QJsonObject o = el.toObject();
        model.spawnEnemy(
            o["health"].toInt(),
            o["x"].toInt(),
            o["y"].toInt(),
            o["damage"].toInt());
    }
}

QJsonArray GameSerializer::serializeArchers(GameModel& model) const
{
    QJsonArray arr;
    for (auto& e : model.getArchers()) {
        QJsonObject obj;
        obj["health"] = e->getHealth();
        obj["x"] = e->getXPos();
        obj["y"] = e->getYPos();
        obj["damage"] = e->getDamage();
        arr.append(obj);
    }
    return arr;
}

void GameSerializer::deserializeArchers(GameModel& model, const QJsonArray& j)
{
    for (auto el : j) {
        QJsonObject o = el.toObject();
        model.spawnArcher(
            o["x"].toInt(),
            o["y"].toInt(),
            o["health"].toInt(),
            o["damage"].toInt());
    }
}

QJsonObject GameSerializer::serializeBoard(GameModel& model) const
{
    QJsonObject jBoard;

    GameBoard& board = model.getBoard();
    int size = board.getSize();
    jBoard["size"] = size;

    QJsonArray rows;
    for (int x = 0; x < size; ++x) {
        QJsonArray row;
        for (int y = 0; y < size; ++y) {
            CellType t = board.currentCell(x,y).getType();
            row.append(static_cast<int>(t));
        }
        rows.append(row);
    }
    jBoard["cells"] = rows;
    return jBoard;
}

void GameSerializer::deserializeBoard(GameModel& model, const QJsonObject& j)
{
    int size = j["size"].toInt();

    EventDispatcher::instance().notify(LevelLoadedEvent(size));

    GameBoard newBoard(size);

    QJsonArray rows = j["cells"].toArray();

    for (int x = 0; x < size; ++x) {
        QJsonArray row = rows[x].toArray();
        for (int y = 0; y < size; ++y) {
            int t = row[y].toInt(); //
            newBoard.currentCell(x, y).setType(static_cast<CellType>(t));
            if (newBoard.currentCell(x, y).getType() == CellType::POISON_TRAP)
            {
                newBoard.addObserver(std::make_unique<PoisonTrap>(SpellConfig::PoisonTrapDamage, model.getEnemies(), x, y));
            }
            if (newBoard.currentCell(x, y).getType() == CellType::HEALTH_POINT)
            {
                newBoard.addObserver(std::make_unique<HealthPoint>(model.getPlayers(), BoardConfig::HealthPointPos.first, BoardConfig::HealthPointPos.second));
            }
            EventDispatcher::instance().notify(NewCellEvent(static_cast<CellType>(t), std::pair<int, int>(x, y)));
        }
    }
    model.getBoard() = std::move(newBoard);
}

QJsonObject GameSerializer::serializeTower(GameModel &model) const
{
    QJsonObject jTower;
    auto& tower = model.getTower();
    jTower["health"] = tower.getHealth();
    return jTower;
}

void GameSerializer::deserializeTower(GameModel &model, const QJsonObject &j)
{
    auto& tower = model.getTower();
    tower.kill();
    tower.addHealth(j["health"].toInt());

}

QJsonObject GameSerializer::serializeBuilding(GameModel &model) const
{
    QJsonObject jBuilding;
    auto& building = model.getBuilding();
    jBuilding["spawn_interval"] = building.getSpawnInterval();
    return jBuilding;
}

void GameSerializer::deserializeBuilding(GameModel &model, const QJsonObject &j)
{
    auto& building = model.getBuilding();
    building.getSpawnInterval() = j["spawn_interval"].toInt();
}

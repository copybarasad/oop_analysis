#include "gameloop.hpp"
#include "../building/spawner-building.hpp"
#include "../building/spell-building.hpp"
#include "../map/euclidean-map.hpp"
#include "message/move-message.hpp"
#include "message/spell-message.hpp"

GameLoop::GameLoop() {
    m_Level = 0;

    m_WeaponFactory = new WeaponFactory();
    m_SpellFactory = new SpellFactory();
}

void GameLoop::run() {
    while (true) {
        runOnce();
    }
}

void GameLoop::runOnce() {
    std::cout << "█▄▄ █▀█ ▄▀█ █ █▄░█ █▀▄ ▄▀█ █▀▄▀█ ▄▀█ █▀▀ █▀▀" << std::endl;
    std::cout << "█▄█ █▀▄ █▀█ █ █░▀█ █▄▀ █▀█ █░▀░█ █▀█ █▄█ ██▄" << std::endl;
    std::cout << std::endl;
    std::cout << "[1] 💊 New Game" << std::endl;
    std::cout << "[2] 🥀 Load Save" << std::endl;
    std::cout << "[3] 📡 Quit" << std::endl;

    m_Level = 0;

    Input input =
        Input()
            .quitOnEOF("bye!")
            .withReject("Enter a digit, baka (caught ur ahh in 4K 📸)")
            .withOption("1")
            .withOption("2")
            .withOption("3");

    while (true) {
        std::string choice = input.collect();

        if (choice == "1") {
            m_Player = new Player(10, 10, 5);
            m_Player->setWeapon({WeaponType::Melee, 2});
            break;
        }

        if (choice == "2") {
            if (tryLoad()) {
                break;
            }
        }

        if (choice == "3") {
            std::exit(0);
        }
    }

    runSession();
}

void GameLoop::runSession() {
    // Unfinished level from save.
    if (m_Level != 0) {
        if (!runLevel()) {
            onLose();
            return;
        }
        levelTransition();
        if (hasNextLevel()) {
            suggestUpgrades();
        }
    }

    while (hasNextLevel()) {
        loadNextLevel();
        if (!runLevel()) {
            onLose();
            return;
        };
        levelTransition();

        if (hasNextLevel()) {
            suggestUpgrades();
        }
    }

    onWin();
}

bool GameLoop::runLevel() {
    while (true) {
        render();
        playerMove();
        allyMove();

        if (!hasAliveEnemies()) {
            return true;
        }

        enemyMove();
        buildingMove();

        if (!m_Player->isAlive()) {
            return false;
        }

        collectSpawnedCreatures();
        finalizeMove();
    }
}

void GameLoop::onLose() {
    std::cout << "\033[2J\033[1;1H" << std::endl;
    std::cout << "Skill issue 😭" << std::endl;
    std::cout << std::endl;
    std::cout << " _________" << std::endl;
    std::cout << "/         \\" << std::endl;
    std::cout << "|  R I P  |" << std::endl;
    std::cout << "|         |" << std::endl;
    std::cout << "|         |" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "\033[2J\033[1;1H" << std::endl;
}

void GameLoop::onWin() {
    std::cout << "\033[2J\033[1;1H" << std::endl;

    static const char *strings[] = {
        "I will transform into a magical girl",
        "It's my committed declaration",
        "I will transform into a magical girl",
        "And my vodka bottle finally will actually help me",
        "I will transform into a magical girl",
        "I will not lose to hesitation",
        "And with my powers and my frail will to mend",
        "I'll prove that nothing is doomed yet",
    };

    constexpr size_t count = sizeof(strings) / sizeof(strings[0]);

    for (size_t i = 0; i < count; ++i) {
        std::cout << strings[i] << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "\033[2J\033[1;1H" << std::endl;
}

bool GameLoop::hasNextLevel() { return m_Level < 3; }

void GameLoop::loadNextLevel() {
    if (!hasNextLevel()) {
        throw std::domain_error("no more levels");
    }

    switch (m_Level) {
    case 0:
        loadLevel1();
        break;
    case 1:
        loadLevel2();
        break;
    case 2:
        loadLevel3();
        break;
    };
}

void GameLoop::loadLevel1() {
    m_Map = new EuclideanMap(10, 10);
    m_Enemies.clear();
    m_Allies.clear();
    m_Buildings.clear();

    m_Enemies.push_back(new Enemy(2, 2));
    m_Enemies.push_back(new Enemy(3, 3));

    m_Enemies[0]->setPosition({0, 7});
    m_Enemies[1]->setPosition({7, 9});

    for (int i = 0; i < m_Enemies.size(); i++) {
        m_Enemies[i]->setMap(m_Map);
        m_Enemies[i]->setWeapon({WeaponType::Melee, 1});
        m_Map->at(m_Enemies[i]->getPosition())->occupy(m_Enemies[i]);
    }

    m_Player->setMap(m_Map);
    m_Map->at({0, 0})->occupy(m_Player);
    m_Player->setPosition({0, 0});
}

void GameLoop::loadLevel2() {
    delete m_Map;
    m_Enemies.clear();
    m_Allies.clear();
    m_Buildings.clear();

    m_Map = new EuclideanMap(15, 15);

    m_Enemies.push_back(new Enemy(6, 6));
    m_Enemies.push_back(new Enemy(6, 6));
    m_Enemies.push_back(new Enemy(6, 6));

    m_Enemies[0]->setPosition({0, 7});
    m_Enemies[1]->setPosition({5, 14});
    m_Enemies[2]->setPosition({14, 6});

    for (int i = 0; i < m_Enemies.size(); i++) {
        m_Enemies[i]->setMap(m_Map);
        m_Enemies[i]->setWeapon({WeaponType::Melee, 3});
        m_Map->at(m_Enemies[i]->getPosition())->occupy(m_Enemies[i]);
    }

    SpawnerBuilding *spawner = new SpawnerBuilding(10);
    m_Buildings.push_back(spawner);

    Enemy *summon = new Enemy(3, 3);
    summon->setMap(m_Map);
    summon->setWeapon({WeaponType::Melee, 2});
    spawner->setMap(m_Map);
    spawner->setEnemy(summon);
    spawner->setPosition({4, 7});
    m_Map->at({4, 7})->build(spawner);

    m_Player->setMap(m_Map);
    m_Map->at({0, 0})->occupy(m_Player);
    m_Player->setPosition({0, 0});
}

void GameLoop::loadLevel3() {
    delete m_Map;
    m_Enemies.clear();
    m_Allies.clear();
    m_Buildings.clear();
    m_Map = new EuclideanMap(20, 20);

    m_Enemies.push_back(new Enemy(8, 8));
    m_Enemies.push_back(new Enemy(8, 8));
    m_Enemies.push_back(new Enemy(8, 8));
    m_Enemies.push_back(new Enemy(8, 8));
    m_Enemies.push_back(new Enemy(8, 8));

    m_Enemies[0]->setPosition({0, 5});
    m_Enemies[1]->setPosition({5, 19});
    m_Enemies[2]->setPosition({0, 10});
    m_Enemies[3]->setPosition({14, 0});
    m_Enemies[4]->setPosition({13, 19});

    for (int i = 0; i < m_Enemies.size(); i++) {
        m_Enemies[i]->setMap(m_Map);
        m_Enemies[i]->setWeapon({WeaponType::Melee, 4});
        m_Map->at(m_Enemies[i]->getPosition())->occupy(m_Enemies[i]);
    }

    SpellBuilding *tower = new SpellBuilding(10);
    m_Buildings.push_back(tower);
    tower->setMap(m_Map);
    tower->setSpell(new DirectDamageSpell(5, 5));
    tower->setTarget(m_Player);
    tower->setPosition({19, 19});
    m_Map->at({19, 19})->build(tower);

    m_Player->setMap(m_Map);
    m_Map->at({0, 0})->occupy(m_Player);
    m_Player->setPosition({0, 0});
}

void GameLoop::playerMove() {
    receiveInput()->handle(m_Player);
    mapUpdate();
    m_Player->update();
}

Field *GameLoop::selectField() {
    int x, y;
    Input input = Input().quitOnEOF("bye!").withPrompt("x y >> ");

    while (true) {
        std::istringstream iss(input.collect());

        if (!(iss >> x >> y)) {
            std::cout << "These aren't digits lil bro 🐰⏱️" << std::endl;
            continue;
        }

        Position pos = {x - 1, y - 1};

        if (m_Map->outOfBounds(pos)) {
            std::cout << "Bruh u forgor ur glasses? Out of bounds 🤓"
                      << std::endl;
            continue;
        }

        return m_Map->at(pos);
    }
}

bool GameLoop::hasAliveEnemies() {
    for (Enemy *e : m_Enemies) {
        if (e->isAlive()) {
            return true;
        }
    }
    return false;
}

void GameLoop::allyMove() {
    for (Ally *a : m_Allies) {
        if (a->isAlive()) {
            a->makeRandomMove();
        }
    }
    mapUpdate();
}

void GameLoop::enemyMove() {
    for (Enemy *e : m_Enemies) {
        if (e->isAlive()) {
            e->makeRandomMove();
            e->update();
        }
    }
    mapUpdate();
}

void GameLoop::buildingMove() {
    for (Building *b : m_Buildings) {
        if (b->isAlive()) {
            b->update();
        }
    }
    mapUpdate();
}

void GameLoop::finalizeMove() {
    size_t i = 0;
    while (i < m_Allies.size()) {
        if (!m_Allies[i]->isAlive()) {
            m_Allies.erase(m_Allies.begin() + i);
        } else {
            i++;
        }
    }

    i = 0;
    while (i < m_Enemies.size()) {
        if (!m_Enemies[i]->isAlive()) {
            m_Enemies.erase(m_Enemies.begin() + i);
        } else {
            i++;
        }
    }

    i = 0;
    while (i < m_Buildings.size()) {
        if (!m_Buildings[i]->isAlive()) {
            m_Buildings.erase(m_Buildings.begin() + i);
        } else {
            i++;
        }
    }
}

void GameLoop::mapUpdate() { m_Map->update(); }

void GameLoop::render() {
    std::cout << "\033[2J\033[1;1H" << std::endl;
    std::cout << m_Player->toString() << std::endl;
    std::cout << m_Map->toString() << std::endl;
}

Message *GameLoop::receiveInput() {
    Input input = Input()
                      .quitOnEOF("bye!")
                      .withReject("Bro ts pmo fr 😭🙏 (rtfm: 'help')")
                      .withOption("w")
                      .withOption("a")
                      .withOption("s")
                      .withOption("d")
                      .withOption("1")
                      .withOption("2")
                      .withOption("3")
                      .withOption("4")
                      .withOption("5")
                      .withOption("kyo")
                      .withOption("save")
                      .withOption("load")
                      .withOption("help");

    while (true) {
        std::string ch = input.collect();

        if (ch == "w") {
            return new MoveMessage(Direction::North);
        }

        if (ch == "a") {
            return new MoveMessage(Direction::West);
        }

        if (ch == "s") {
            return new MoveMessage(Direction::South);
        }

        if (ch == "d") {
            return new MoveMessage(Direction::East);
        }

        if (ch == "1" || ch == "2" || ch == "3" || ch == "4" || ch == "5") {
            return new SpellMessage(std::stoi(ch) - 1, selectField());
        }

        if (ch == "save") {
            trySave();
            continue;
        }

        if (ch == "load") {
            tryLoad();
            continue;
        }

        if (ch == "help") {
            help();
            continue;
        }

        if (ch == "kyo") {
            kyo();
            continue;
        }
    }
}

bytedump GameLoop::dump() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(m_Level));

    return bytes;
}

bytedump GameLoop::dumpAllies() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(m_Allies.size()));
    for (Ally *ally : m_Allies) {
        bytedump allyBytes = ally->dump();
        bytes.insert(bytes.end(), allyBytes.begin(), allyBytes.end());
    }

    return bytes;
}

bytedump GameLoop::dumpEnemies() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(m_Enemies.size()));
    for (Enemy *enemy : m_Enemies) {
        bytedump enemyBytes = enemy->dump();
        bytes.insert(bytes.end(), enemyBytes.begin(), enemyBytes.end());
    }

    return bytes;
}

bytedump GameLoop::dumpBuildings() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(m_Buildings.size()));
    for (Building *building : m_Buildings) {
        bytedump buildingBytes = building->dump();
        bytes.insert(bytes.end(), buildingBytes.begin(), buildingBytes.end());
    }

    return bytes;
}

void GameLoop::save() {
    Save *save = new Save();

    save->write(dump());
    save->write(m_Map->dump());
    save->write(m_Player->dump());
    save->write(dumpAllies());
    save->write(dumpEnemies());
    save->write(dumpBuildings());

    fs::path saveDsc = XDG()
                           .inDataHome()
                           .withAppName("braindamage")
                           .withFileName("save.bin")
                           .build();

    save->save(saveDsc);
}

void GameLoop::loadSelf(bytedump bytes) {
    if (bytes.size() != 1) {
        throw std::length_error("gameloop dump must contain a single byte");
    }

    m_Level = static_cast<int>(bytes[0]);
}

void GameLoop::loadMap(bytedump bytes) {
    delete m_Map;
    m_Map = new EuclideanMap(bytes);
}

void GameLoop::loadPlayer(bytedump bytes) {
    delete m_Player;
    m_Player = new Player(bytes);
    m_Player->setMap(m_Map);
    m_Map->at(m_Player->getPosition())->occupy(m_Player);
}

void GameLoop::loadAllies(bytedump bytes) {
    if (bytes.size() < 1) {
        throw std::length_error("allies dump must contain at least 1 byte");
    }

    m_Allies.clear();

    int length = static_cast<int>(bytes[0]);
    int offset = 1;

    if (bytes.size() != 1 + length * 7) {
        throw std::length_error("allies dump size mismatch");
    }

    for (int i = 0; i < length; i++) {
        bytedump allyBytes(bytes.begin() + offset, bytes.begin() + offset + 7);
        Ally *ally = new Ally(allyBytes);
        ally->setMap(m_Map);
        m_Map->at(ally->getPosition())->occupy(ally);
        m_Allies.push_back(ally);
        offset += 7;
    }
}

void GameLoop::loadEnemies(bytedump bytes) {
    if (bytes.size() < 1) {
        throw std::length_error("enemies dump must contain at least 1 byte");
    }

    m_Enemies.clear();

    int length = static_cast<int>(bytes[0]);
    int offset = 1;

    if (bytes.size() != 1 + length * 7) {
        throw std::length_error("enemies dump size mismatch");
    }

    for (int i = 0; i < length; i++) {
        bytedump enemyBytes(bytes.begin() + offset, bytes.begin() + offset + 7);
        Enemy *enemy = new Enemy(enemyBytes);
        enemy->setMap(m_Map);
        m_Map->at(enemy->getPosition())->occupy(enemy);
        m_Enemies.push_back(enemy);
        offset += 7;
    }
}

void GameLoop::loadBuildings(bytedump bytes) {
    if (bytes.size() < 1) {
        throw std::length_error("buildings dump must contain at least 1 byte");
    }

    m_Buildings.clear();

    int length = static_cast<int>(bytes[0]);
    int offset = 1;

    if (bytes.size() != 1 + length * 16) {
        throw std::length_error("buildings dump size mismatch");
    }

    for (int i = 0; i < length; i++) {
        bytedump buildingBytes(bytes.begin() + offset,
                               bytes.begin() + offset + 16);
        Building *building = Building::fromByteDump(buildingBytes);
        building->setMap(m_Map);
        m_Map->at(building->getPosition())->build(building);
        m_Buildings.push_back(building);
        offset += 16;
    }
}

void GameLoop::load() {
    fs::path saveSrc = XDG()
                           .inDataHome()
                           .withAppName("braindamage")
                           .withFileName("save.bin")
                           .build();

    Save *save = new Save(saveSrc);

    std::vector<bytedump> parts = save->parts();

    loadSelf(parts[0]);
    loadMap(parts[1]);
    loadPlayer(parts[2]);
    loadAllies(parts[3]);
    loadEnemies(parts[4]);
    loadBuildings(parts[5]);
    render();
}

bool GameLoop::trySave() {
    try {
        save();
        return true;
    } catch (std::runtime_error e) {
        std::cerr << "Cannot open save file!" << std::endl;
        std::cerr << e.what() << std::endl;
    } catch (std::exception e) {
        std::cerr << "Cannot save" << std::endl;
    }

    return false;
}

bool GameLoop::tryLoad() {
    try {
        load();
        return true;
    } catch (std::runtime_error e) {
        std::cerr << "Cannot open save file!" << std::endl;
        std::cerr << e.what() << std::endl;
    } catch (std::length_error e) {
        std::cerr << "Save file is corrupted!" << std::endl;
        std::cerr << e.what() << std::endl;
    } catch (InvalidSignatureError e) {
        std::cerr << "Save file is corrupted!" << std::endl;
        std::cerr << e.what() << std::endl;
    } catch (std::exception e) {
        std::cerr << "Save cannot be loaded" << std::endl;
    }

    return false;
}

void GameLoop::levelTransition() {
    m_Player->healFullHP();
    m_Player->spellHand()->removeHalf();
    m_SpellFactory->incrementLevel();
    m_WeaponFactory->incrementLevel();
    m_Level++;
}

void GameLoop::suggestUpgrades() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(3, 7);

    int maxHPIncrement = dis(gen);
    Weapon newMeleeWeapon = m_WeaponFactory->createMelee();
    Spell *newSpell = m_SpellFactory->create();

    std::cout << "Pick a reward:" << std::endl;
    std::cout << "[1] 🫀 Increase max HP  (+" << maxHPIncrement << ")"
              << std::endl;
    std::cout << "[2] 🔪 New melee weapon (damage: " << newMeleeWeapon.damage
              << ")" << std::endl;
    std::cout << "[3] ✨ Spell (" << newSpell->toString() << ")" << std::endl;

    std::string line = Input()
                           .quitOnEOF("bye!")
                           .withReject("r u dumb? pick {1, 2, 3} rn!")
                           .withOption("1")
                           .withOption("2")
                           .withOption("3")
                           .collect();

    if (line == "1") {
        m_Player->increaseMaxHP(maxHPIncrement);
        m_Player->healFullHP();
    }

    if (line == "2") {
        m_Player->setWeapon(newMeleeWeapon);
    }

    if (line == "3") {
        m_Player->spellHand()->addSpell(newSpell);
    }
}

void GameLoop::collectSpawnedCreatures() {
    std::vector<Ally *> newAllies = m_Map->extractSpawnedAllies();
    m_Allies.insert(m_Allies.end(), newAllies.begin(), newAllies.end());

    std::vector<Enemy *> newEnemies = m_Map->extractSpawnedEnemies();
    m_Enemies.insert(m_Enemies.end(), newEnemies.begin(), newEnemies.end());
}

void GameLoop::help() {
    std::cout << "[w]    - Up" << std::endl;
    std::cout << "[a]    - Left" << std::endl;
    std::cout << "[s]    - Down" << std::endl;
    std::cout << "[d]    - Right" << std::endl;
    std::cout << "[1-5]  - Cast Spell" << std::endl;
    std::cout << "[save] - Save Game" << std::endl;
    std::cout << "[load] - Load Save" << std::endl;
    std::cout << "[help] - welp..." << std::endl;
}

void GameLoop::kyo() {
    // clang-format off
    std::cout << std::endl;
    std::cout << "..................+=+===-=+#--++=+---++=---=+-++-=++-+::::-%-:::@.@#........@@@." << std::endl;
    std::cout << "................#==-*=+--*+*---+++-:-=++---@+=+=-%++@:-::::::-*@...--@....*...@." << std::endl;
    std::cout << "...............==.-=++-::-@==:--=+-*-#-::--------------:-::::::....@--@.@....@.." << std::endl;
    std::cout << ".............::=.:-:=++--@@#.:@=.:::::::::----------::::::::-::...:-:--:....-=.@" << std::endl;
    std::cout << "...........@-@-=.@---#:::::::::::::::::::::------------%:::::::@..:-@--@...-@..." << std::endl;
    std::cout << "..........--:::---:-:::::::....::::::::::::=-------+-----:::::::..---#-@..-:.:::" << std::endl;
    std::cout << ".........:-::-=:::=*::::.:::::::::::::::::::@-------+-----:::::::-----#----@::::" << std::endl;
    std::cout << "...@..*.@-:-=*:..+*::::......*:::::::::::::::@-------+--------%=--@:@:.---@@+---" << std::endl;
    std::cout << "......@@---+-:..*+:::::@.......@.:::::::::::::--=----=+-----=::::::@....@-=++---" << std::endl;
    std::cout << "..@...%---+#:.:%=%::::::.@...+::::@::.::::::::@--=----+=-----=:::::@.....-*=++--" << std::endl;
    std::cout << ".@....*:-+=:...-+*:..::::::@%:#:::%:@@.:::::::.--+-----+--::--::::::%....%:::@+=" << std::endl;
    std::cout << "@:...:#--+:...@-+::...:::::::.@::::@...@:::::::--=-:-:-.=-----%::---::::-:::--::" << std::endl;
    std::cout << ":-@::-#-+=:...--+:.....:.:..::.:+*........:::::@-%=:::@@:--::--@:::::::----::::-" << std::endl;
    std::cout << "---@----+@....-:=:.................@.....@:....@-#%@+::-:+--::-%::::::@-----:-::" << std::endl;
    std::cout << ".@@..@--+:....---@...................@:.=..:..@@:.@::::-@.--:::-::::-@::--------" << std::endl;
    std::cout << "-+@..---+:....=--#.............................@*.%:::::@:-::---@--::::::+--@---" << std::endl;
    std::cout << "+++@:-:-=:..:@@-::.:.....:.....................@.......:@.-:-::-@-:::::::--#----" << std::endl;
    std::cout << "++=-:-:-=:.....%-:#........................:=...+@@@@@#.......%--::::::::--:---@" << std::endl;
    std::cout << "=:------=@.......%-=......................-:.:@@..#:...=@@@@...:-:::::::--@:---." << std::endl;
    std::cout << ":----=----:........#....%:.:................*%:#@=+........@@@.:-*::@@+#-:::---@" << std::endl;
    std::cout << "--------*=@................@:................::@@+.........::@@%:::::::::::-:--=" << std::endl;
    std::cout << "-----+:--.#@.....@@@@@@@@@...................:+@**%........::@..::::@=-%--:@:---" << std::endl;
    std::cout << ":----:::::.....@@@::@+....@@.................:*@##%#@:....@.....::@=#---%:::+--@" << std::endl;
    std::cout << "-----::%-@:.+:@@::@@+=........................:@##%####%%*@......:#------@:::@-%" << std::endl;
    std::cout << "--:--::@-::::@@.::@@**.....:...................@#++%+%+%#+@-.::::-:*====@::::=@." << std::endl;
    std::cout << "---::::@@-::@@::::@########%@.....................==@%@@==@:::::::-@*=====:::#::" << std::endl;
    std::cout << "-:-@:::::--@:.@:::@@###%++@+@...................@%=====---@:::::::-@@@@==::.@:::" << std::endl;
    std::cout << "-:-::::@--@=*:::..@@@.@*+%%+@-................::@@@@+:::::::::::::-@@@=@...@:---" << std::endl;
    std::cout << ":-@::::+:-+==:::::....=@@@==@:...............:::::::::::::-:::::::-@@.....@--=++" << std::endl;
    std::cout << ":-:...*:---=@@::::::@@==-#@@-:...............:::::::::::::::-:::::@@.....:::---:" << std::endl;
    std::cout << ":%....@:---:@@::::::::::::::::...............:::::::::::::::::::::@-..%:::::::::" << std::endl;
    std::cout << "-@:...*:.=--@@::::::::::::::::......--.......:::::::::::::::::::::@::*....::::::" << std::endl;
    std::cout << "-:::..::...@-@@::::+:::::::::::...............:::::::::::::::::::.@-.@...:::::::" << std::endl;
    std::cout << "+-::::::.....@::::::::::::::::.................::::::::::::::::::-:-=......:::::" << std::endl;
    std::cout << "@----:::......@.::::::::::::::........................::::..:...:..-@..........." << std::endl;
    std::cout << "@-----::........::::::::::::...........*.@@...................@..#.-@..........." << std::endl;
    std::cout << "=-----:::......:..:::::::::.................................@....@.-@..........." << std::endl;
    std::cout << "-----:::-::::::@::@.::::..................................@==:...@.--..........:" << std::endl;
    std::cout << "------:--------@--::+@.................................@+====@..-..@..........:-" << std::endl;
    std::cout << "-@----@---------:-::::::@...........................@-=======:@-@.@@.........@--" << std::endl;
    std::cout << "-%=---@---------@-:::::::::::@:.................#+==========:::@=@.-@@------*---" << std::endl;
    std::cout << "---==--#--------@--:::::::::::::::::+@#:...%#@============-:::*@..--..@:----@---" << std::endl;
    std::cout << ":-@-===:--======+---:::::---::::::::::::::::--+==========:::::*..--...-.....#---" << std::endl;
    std::cout << ":-:-=====@===+++=+-::::::.:--------------....-#:======-::::::.@.:-...-......@@:-" << std::endl;
    std::cout << std::endl;
    // clang-format on
}

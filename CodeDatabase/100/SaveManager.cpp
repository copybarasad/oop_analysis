#include "SaveManager.h"

// Сохранение
bool SaveManager::save(GameState& state, const std::string& filename) {
    try {
        std::ofstream file;
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        file.open(filename);

        // LEVEL 
        file << "LEVEL " << state.getLevel() << "\n"; 

        // FIELD
        const GameField& field = state.getField();
        file << "FIELD " << field.getWidth() << " " << field.getHeight() << "\n";

        for (int y = 0; y < field.getHeight(); y++) {
            for (int x = 0; x < field.getWidth(); x++) {
                const Cell& cell = field.getCell(x, y);
                int stateValue = 0;

                if (!cell.getIsPassable()) stateValue = 0;
                else if (cell.getHasTrap()) stateValue = 3;
                else if (cell.getIsSlowing()) stateValue = 2;
                else stateValue = 1;

                file << stateValue << " ";
            }
            file << "\n";
        }

        // PLAYER
        const Player& player = state.getPlayer();

        file << "PLAYER "
             << player.getX() << " "
             << player.getY() << " "
             << player.getHealth() << " "
             << player.getMaxHealth() << " " //////////////////////
             << player.getDamage() << " "
             << (player.getCombatMode() == CombatMode::Melee ? 0 : 1) << " "
             << player.getScore() << "\n";

        // CHARACTERS
        file << "CHARACTERS " << state.getAllCharacters().size() << "\n";
        for (const auto& character : state.getAllCharacters()) {
            if (character == &state.getPlayer()) continue;
            file << character->getSymbol() << " "
                 << character->getX() << " "
                 << character->getY() << " "
                 << character->getHealth() << " "
                 << character->getDamage() << "\n";
        }

        // ALLIES
        file << "ALLIES " << state.getAllAllies().size() << "\n";
        for (const auto& ally : state.getAllAllies()) {
            file << ally->getSymbol() << " "
                 << ally->getX() << " "
                 << ally->getY() << " "
                 << ally->getHealth() << " "
                 << ally->getDamage() << "\n";
        }

        // BUILDINGS
        file << "BUILDINGS " << state.getAllBuildings().size() << "\n";
        for (const auto& b : state.getAllBuildings()) {
            file << b->getSymbol() << " "
                 << b->getX() << " "
                 << b->getY() << " "
                 << b->getHealth() << "\n";
        }

        // TOWERS
        file << "TOWERS " << state.getAllTowers().size() << "\n";
        for (const auto& t : state.getAllTowers()) {
            file << t->getSymbol() << " "
                 << t->getX() << " "
                 << t->getY() << " "
                 << t->getHealth() << "\n";
        }

        // HAND
        const auto& hand = state.getHand().getAllSpells();
        file << "HAND " << hand.size() << "\n";

        for (const auto& spell : hand) {
            char symbol = spell->getSymbol();
            int level = spell->getLevel();
            int range = spell->getRange();

            if (symbol == 'D') {
                auto* s = dynamic_cast<DirectDamageSpell*>(spell.get());
                file << symbol << " " << level << " " << range << " " << s->getDamage() << " 0\n";
            }
            else if (symbol == 'A') {
                auto* s = dynamic_cast<AreaDamageSpell*>(spell.get());
                file << symbol << " " << level << " " << range << " " << s->getDamage() << " " << s->getAreaSize() << "\n";
            }
            else if (symbol == 'T') {
                auto* s = dynamic_cast<TrapSpell*>(spell.get());
                file << symbol << " " << level << " " << range << " " << s->getDamage() << " 0\n";
            }
            else if (symbol == 'a') {
                auto* s = dynamic_cast<SpawnAllySpell*>(spell.get());
                file << symbol << " " << level << " " << range << " " << s->getAllyDamage() << " " << s->getAllyHealth() << "\n";
            }
            else if (symbol == 'U') {
                file << symbol << " " << level << " " << range << " 0 0\n";
            }
            else {
                throw std::runtime_error("Unknown spell type while saving!");
            }
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << Colors::DARK_RED << "Save error: Invalid file or corrupted data" << Colors::RESET << std::endl;
        return false;
    }
}

// Сохранение
bool SaveManager::load(GameState& state, const std::string& filename) {
    try {
        std::ifstream file;
        file.exceptions(std::ifstream::badbit);
        file.open(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        state.clearGameState();

        std::string header;

        // LEVEL
        file >> header;
        if (header != "LEVEL") throw std::invalid_argument("Invalid file: expected LEVEL");
        int level;
        file >> level;
        if (level < 1) throw std::invalid_argument("Invalid data: LEVEL must be > 0");

        // FIELD
        file >> header;
        if (header != "FIELD") throw std::invalid_argument("Invalid file: expected FIELD");

        int width, height;
        if (!(file >> width >> height)) throw std::invalid_argument("FIELD data corrupted");
        if (width <= 0 || height <= 0) throw std::invalid_argument("Invalid data: FIELD dimensions must be > 0");

        state.clearGameState();
        new (&state) GameState(width, height, 100, 20); 
        state.setLevel(level);

        {
            GameField& field = const_cast<GameField&>(state.getField());

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int value;
                    if (!(file >> value))
                        throw std::invalid_argument("Corrupted FIELD cell data");
                    
                    if (value < 0 || value > 3)
                        throw std::invalid_argument("Invalid data: FIELD cell value must be 0-3");

                    field.setPassable(x, y, value != 0);
                    field.setSlowing(x, y, value == 2);
                    field.setTrap(x, y, value == 3);
                }
            }
        }

        // PLAYER
        file >> header;
        if (header != "PLAYER") throw std::invalid_argument("Invalid file: expected PLAYER");

        {
            int x, y, health, maxHealth, damage, mode, score;
            if (!(file >> x >> y >> health >> maxHealth >> damage >> mode >> score))
                throw std::invalid_argument("PLAYER data corrupted");

            if (x < 0 || x >= width || y < 0 || y >= height)
                throw std::invalid_argument("Invalid data: PLAYER position out of bounds");
            if (health <= 0 || maxHealth <= 0)
                throw std::invalid_argument("Invalid data: PLAYER health must be > 0");
            if (damage <= 0)
                throw std::invalid_argument("Invalid data: PLAYER damage must be > 0");
            if (!(mode == 0 || mode == 1))
                throw std::invalid_argument("Invalid data: PLAYER combat mode must be 0 or 1");
            if (score < 0)
                throw std::invalid_argument("Invalid data: PLAYER score must be >= 0");

            Player& p = state.getPlayer();
            p.setPosition(x, y);
            p.setHealth(health);
            p.setMaxHealth(maxHealth);
            p.setDamage(damage);
            p.setCombatMode(mode);
            p.setScore(score);

            state.addCharacter(&p);
        }

        // CHARACTERS
        file >> header;
        if (header != "CHARACTERS")
            throw std::invalid_argument("Invalid file: expected CHARACTERS");

        int count;
        file >> count;
        if (count < 0) throw std::invalid_argument("Invalid data: CHARACTERS count must be >= 0");

        for (int i = 1; i < count; i++) {
            char symbol;
            int x, y, health, damage;
            if (!(file >> symbol >> x >> y >> health >> damage))
                throw std::invalid_argument("Corrupted CHARACTER entry");

            if (x < 0 || x >= width || y < 0 || y >= height)
                throw std::invalid_argument("Invalid data: CHARACTER position out of bounds");
            if (health < 0)
                throw std::invalid_argument("Invalid data: CHARACTER health must be > 0");
            if (damage <= 0)
                throw std::invalid_argument("Invalid data: CHARACTER damage must be > 0");

            state.addCharacter(new Enemy(health, damage, x, y, symbol));
        }

        // ALLIES
        file >> header;
        if (header != "ALLIES") throw std::invalid_argument("Invalid file: expected ALLIES");

        file >> count;
        if (count < 0) throw std::invalid_argument("Invalid data: ALLIES count must be >= 0");

        for (int i = 0; i < count; i++) {
            char symbol;
            int x, y, health, damage;

            if (!(file >> symbol >> x >> y >> health >> damage))
                throw std::invalid_argument("Corrupted ALLY entry");

            if (x < 0 || x >= width || y < 0 || y >= height)
                throw std::invalid_argument("Invalid data: ALLY position out of bounds");
            if (health < 0)
                throw std::invalid_argument("Invalid data: ALLY health must be > 0");
            if (damage <= 0)
                throw std::invalid_argument("Invalid data: ALLY damage must be > 0");

            state.addAlly(new Ally(health, damage, x, y, symbol));
        }

        // BUILDINGS
        file >> header;
        if (header != "BUILDINGS") throw std::invalid_argument("Invalid file: expected BUILDINGS");

        file >> count;
        if (count < 0) throw std::invalid_argument("Invalid data: BUILDINGS count must be >= 0");

        for (int i = 0; i < count; i++) {
            char symbol;
            int x, y, health;

            if (!(file >> symbol >> x >> y >> health))
                throw std::invalid_argument("Corrupted BUILDING entry");
            if (x < 0 || x >= width || y < 0 || y >= height)
                throw std::invalid_argument("Invalid data: BUILDING position out of bounds");
            if (health < 0)
                throw std::invalid_argument("Invalid data: BUILDING health must be > 0");

            state.addBuilding(new EnemyBuilding(health, x, y, 10, symbol));
        }

        // TOWERS
        file >> header;
        if (header != "TOWERS") throw std::invalid_argument("Invalid file: expected TOWERS");

        file >> count;
        if (count < 0) throw std::invalid_argument("Invalid data: TOWERS count must be >= 0");

        for (int i = 0; i < count; i++) {
            char symbol;
            int x, y, health;

            if (!(file >> symbol >> x >> y >> health))
                throw std::invalid_argument("Corrupted TOWER entry");

            if (x < 0 || x >= width || y < 0 || y >= height)
                throw std::invalid_argument("Invalid data: TOWER position out of bounds");
            if (health < 0)
                throw std::invalid_argument("Invalid data: TOWER health must be > 0");

            state.addTower(new EnemyTower(health, x, y, symbol));
        }

        // HAND
        file >> header;
        if (header != "HAND") throw std::invalid_argument("Invalid file: expected HAND");

        file >> count;
        if (count < 0) throw std::invalid_argument("Invalid data: HAND count must be >= 0");

        state.clearHand();

        for (int i = 0; i < count; i++) {
            char symbol;
            int level, range, damage, radius;

            if (!(file >> symbol >> level >> range >> damage >> radius))
                throw std::invalid_argument("Corrupted SPELL entry");

            if (level < 1) throw std::invalid_argument("Invalid data: SPELL level must be > 0");
            if (range < 0) throw std::invalid_argument("Invalid data: SPELL range must be >= 0");
            if (damage < 0) throw std::invalid_argument("Invalid data: SPELL damage must be >= 0");
            if (radius < 0) throw std::invalid_argument("Invalid data: SPELL radius must be >= 0");

            std::shared_ptr<Spell> spell;

            if (symbol == 'D')
                spell = std::make_shared<DirectDamageSpell>(damage, range);
            else if (symbol == 'A')
                spell = std::make_shared<AreaDamageSpell>(damage, range, radius);
            else if (symbol == 'T')
                spell = std::make_shared<TrapSpell>(damage, range);
            else if (symbol == 'a')
                spell = std::make_shared<SpawnAllySpell>(range, radius, damage, symbol);
            else if (symbol == 'U')
                spell = std::make_shared<UpgradeSpell>(damage);
            else
                throw std::invalid_argument("Unknown spell type");

            for (int l = 1; l < level; l++) spell->upgrade();

            state.addSpellToHand(spell);
        }

        return true;
    }
    
    catch (const std::exception& e) {
        std::cerr << Colors::DARK_RED << "Load error: " << e.what() << Colors::RESET << std::endl;
        return false;
    }
}

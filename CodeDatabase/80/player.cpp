#include "Player.h"
#include "Map.h"  
#include "GameEventManager.h" 
#include <string>
#include <random>

Player::Player() : hand(5), enemies_defeated(0) {
    ind_type = 'H';
    heallpoints = 100;
    damage = 10;
    attack_mode = 0;
    maxHealth = 100;
    spellPower = 10;
    slowed = false;
    hand.AddRandomSpell();

    EVENT_MANAGER.logEvent(
        GameEventType::PLAYER_SPELL_GAINED,
        "Player created with initial spell",
        Logger::createEntityDetails("Player", 0, 0, heallpoints)
    );
}

void Player::AddRandomSpell() {
    hand.AddRandomSpell();

    int spellCount = hand.CountSpells();
    EVENT_MANAGER.logPlayerSpellGained("Random Spell");
    LOG_INFO("Player now has " + std::to_string(spellCount) + " spells");
}

int Player::GetSpellCount() const {
    return hand.CountSpells();
}

void Player::RemoveRandomSpell() {
    int oldCount = hand.CountSpells();

    if (hand.CountSpells() > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, hand.CountSpells() - 1);

        int randomIndex = distrib(gen);
        hand.ChooseSpell(randomIndex);

        std::cout << "One spell has been removed from your hand." << std::endl;

        int newCount = hand.CountSpells();
        LOG_INFO("Spell removed. Spells: " + std::to_string(oldCount) + " -> " + std::to_string(newCount));
    }
    else {
        std::cout << "No spells to remove." << std::endl;
    }
}

void Player::RemoveHalfOfSpells() {
    int currentSpellCount = hand.CountSpells();
    if (currentSpellCount <= 0) {
        std::cout << "No spells to remove." << std::endl;
        return;
    }

    int spellsToRemove = currentSpellCount / 2;
    std::cout << "Removing " << spellsToRemove << " out of " << currentSpellCount << " spells..." << std::endl;

    LOG_INFO("Removing " + std::to_string(spellsToRemove) + " of " + std::to_string(currentSpellCount) + " spells");

    for (int i = 0; i < spellsToRemove; i++) {
        RemoveRandomSpell();
    }

    std::cout << "You now have " << hand.CountSpells() << " spells remaining." << std::endl;

    int remainingSpells = hand.CountSpells();
    LOG_INFO("Spell removal complete. Remaining: " + std::to_string(remainingSpells));
}

int Player::GetAttackMode() {
    return attack_mode;
}

int Player::GetDefeatedCount() {
    return enemies_defeated;
}

void Player::Slowed() {
    slowed = true;
    int oldHP = heallpoints;
    heallpoints -= 10;

    LOG_WARNING("Player slowed and took 10 damage");
    EVENT_MANAGER.logPlayerDamageTaken(10, heallpoints);
}

bool Player::IsSlowed() {
    if (slowed == true) {
        slowed = false;

        LOG_INFO("Slow effect removed from player");
        return true;
    }
    return false;
}

void Player::Spawn(int x, int y, Map& map) {
    this->x = x;
    this->y = y;
    map.SetVal(x, y, ind_type, this);

    LOG_INFO("Player spawned at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    auto details = Logger::createEntityDetails("Player", x, y, heallpoints);
    LOG_EVENT(GameEventType::PLAYER_MOVE, "Player spawned", details);
}

void Player::Move(Map& map) {
}

void Player::Step(char direction, Map& map) {
    int oldX = x;
    int oldY = y;

    int dx, dy;
    GetDirection(direction, dx, dy);

    int new_x = x + dx;
    int new_y = y + dy;

    if (map.CanMoveTo(new_x, new_y)) {
        if (map.GetVal(new_x, new_y) == '*') {
            slowed = true;
            EVENT_MANAGER.logTrapActivated(new_x, new_y, "Player");
            LOG_WARNING("Player stepped on trap at (" + std::to_string(new_x) + ", " + std::to_string(new_y) + ")");
        }

        map.SetVal(x, y, '0', this);
        SetCoord(new_x, new_y);
        map.SetVal(new_x, new_y, ind_type, this);

        EVENT_MANAGER.logPlayerMove(oldX, oldY, new_x, new_y);
        LOG_DEBUG("Player moved: (" + std::to_string(oldX) + "," + std::to_string(oldY) +
            ") -> (" + std::to_string(new_x) + "," + std::to_string(new_y) + ")");
    }
    else {
        LOG_DEBUG("Player attempted to move to blocked cell (" +
            std::to_string(new_x) + "," + std::to_string(new_y) + ")");

        if (map.GetVal(new_x, new_y) == 'e') {
            int oldHP = heallpoints;
            heallpoints -= 10;

            EVENT_MANAGER.logPlayerDamageTaken(10, heallpoints);
            LOG_WARNING("Player collided with enemy and took 10 damage. HP: " +
                std::to_string(oldHP) + " -> " + std::to_string(heallpoints));
        }
    }
}

void Player::Attack(Map& map) {
    char input;
    int dx = 0;
    int dy = 0;
    std::cin >> input;

    bool valid_input = true;
    switch (input) {
    case 'w': dx = -1; dy = 0; break;
    case 'a': dx = 0; dy = -1; break;
    case 's': dx = 1; dy = 0; break;
    case 'd': dx = 0; dy = 1; break;
    default: valid_input = false;
    }

    if (!valid_input) {
        std::cout << "Invalid attack direction! Use w/a/s/d." << std::endl;
        LOG_WARNING("Invalid attack input: " + std::string(1, input));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    int attack_x = x + dx - abs(attack_mode * dx);
    int attack_y = y + dy + abs(attack_mode * dy);

    LOG_DEBUG("Player attacking from (" + std::to_string(x) + "," + std::to_string(y) +
        ") to (" + std::to_string(attack_x) + "," + std::to_string(attack_y) + ")");

    if (attack_x >= 0 && attack_x < map.GetHeight() && attack_y >= 0 && attack_y < map.GetWeight()) {
        char targetCell = map.GetVal(attack_x, attack_y);

        if (targetCell == 'e' || targetCell == 'T' || targetCell == 'B') {
            auto& objects = map.GetObjects();
            auto enemy_it = objects.find({ attack_x, attack_y });
            if (enemy_it != objects.end()) {
                Entity* enemy = enemy_it->second;

                std::string targetType;
                if (targetCell == 'e') targetType = "Enemy";
                else if (targetCell == 'T') targetType = "Tower";
                else if (targetCell == 'B') targetType = "AttackTower";

                int enemyOldHP = enemy->heallpoints;

                LOG_INFO("Player attacking " + targetType + " at (" +
                    std::to_string(attack_x) + "," + std::to_string(attack_y) +
                    ") with damage " + std::to_string(damage));

                EVENT_MANAGER.logPlayerAttack(targetType, damage);

                enemy->TakeDamage(damage, map);

                LOG_DEBUG(targetType + " HP: " + std::to_string(enemyOldHP) +
                    " -> " + std::to_string(enemy->heallpoints));

                if (enemy->heallpoints <= 0) {
                    EnemyDefeated();
                    LOG_INFO(targetType + " defeated at position (" +
                        std::to_string(attack_x) + "," + std::to_string(attack_y) + ")");
                }
            }
        }
        else {
            LOG_DEBUG("Attack missed - no valid target at (" +
                std::to_string(attack_x) + "," + std::to_string(attack_y) + ")");
        }
    }
    else {
        LOG_WARNING("Attack coordinates out of bounds: (" +
            std::to_string(attack_x) + "," + std::to_string(attack_y) + ")");
    }
}

void Player::UseSpell(Map& map) {
    ShowSpells();

    if (hand.CountSpells() > 0) {
        std::cout << "Choose spell to use (1-" << hand.CountSpells() << "): or 0 to cancel ";

        bool correct_input = false;
        int choice;
        while (!correct_input) {
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid spell!" << std::endl;
                LOG_WARNING("Invalid spell selection input");
            }
            else if (choice >= 0 && choice <= hand.CountSpells()) {
                correct_input = true;
            }
            else {
                std::cout << "Invalid spell!" << std::endl;
                LOG_WARNING("Spell index out of range: " + std::to_string(choice));
            }
        }

        if (choice == 0) {
            LOG_INFO("Spell selection cancelled by player");
            return;
        }

        std::unique_ptr<Spell> spell_chosen = hand.ChooseSpell(choice - 1);
        if (!spell_chosen) {
            std::cout << "Failed to get spell!" << std::endl;
            LOG_ERROR("Failed to retrieve spell at index " + std::to_string(choice - 1));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }

        std::string spellInfo = spell_chosen->GetInfo();
        LOG_INFO("Player selected spell: " + spellInfo);

        std::cout << "Input coordinates: ";

        correct_input = false;
        int target_x, target_y;
        while (!correct_input) {
            std::cin >> target_x >> target_y;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Cannot cast spell there!" << std::endl;
                LOG_WARNING("Invalid coordinates input for spell casting");
            }
            else if (spell_chosen->CanCast(x, y, target_x, target_y, map)) {
                LOG_INFO("Casting " + spellInfo + " from (" + std::to_string(x) + "," + std::to_string(y) +
                    ") to (" + std::to_string(target_x) + "," + std::to_string(target_y) + ")");

                EVENT_MANAGER.logPlayerSpellUsed(spellInfo, target_x, target_y);

                spell_chosen->UseSpell(target_x, target_y, map);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                correct_input = true;

                LOG_INFO("Spell cast successfully");
            }
            else {
                std::cout << "Cannot cast spell there! Check coordinates and range. Try again: ";
                LOG_DEBUG("Spell cannot be cast to (" + std::to_string(target_x) + "," +
                    std::to_string(target_y) + ") - range or line of sight issue");
            }
        }
    }
    else {
        std::cout << "No spells available!" << std::endl;
        LOG_WARNING("Player attempted to use spell but has none available");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Player::ShowSpells() {
    int spellCount = hand.CountSpells();

    LOG_DEBUG("Displaying spells. Count: " + std::to_string(spellCount));

    if (spellCount > 0) {
        std::cout << "Your spells:" << std::endl;
        hand.Display();

        for (int i = 0; i < spellCount; i++) {
            LOG_DEBUG("Spell " + std::to_string(i + 1) + ": " + hand.GetSpellInfo(i));
        }
    }
    else {
        std::cout << "You haven't any spells" << std::endl;
        LOG_DEBUG("Player has no spells to display");
    }
}

void Player::ChangeMode() {
    int oldMode = attack_mode;
    attack_mode = (attack_mode + 1) % 2;
    std::cout << "Attack mode has been changed" << std::endl;

    std::string modeName = (attack_mode == 0) ? "Standard" : "Alternate";
    LOG_INFO("Attack mode changed: " + std::to_string(oldMode) + " -> " +
        std::to_string(attack_mode) + " (" + modeName + ")");
}

void Player::EnemyDefeated() {
    int oldCount = enemies_defeated;
    enemies_defeated++;

    LOG_INFO("Enemy defeated! Count: " + std::to_string(oldCount) + " -> " +
        std::to_string(enemies_defeated));

    hand.AddSpellForAchievement(enemies_defeated);

    if (enemies_defeated % 3 == 0) {
        LOG_INFO("Player eligible for achievement reward at " +
            std::to_string(enemies_defeated) + " enemies defeated");
    }
}

void Player::Refresh() {
    int oldHP = heallpoints;
    int oldMaxHP = maxHealth;
    int oldEnemiesDefeated = enemies_defeated;
    int oldSpellCount = hand.CountSpells();

    heallpoints = maxHealth;
    enemies_defeated = 0;
    attack_mode = 0;
    slowed = false;

    hand.Clear();
    hand.AddRandomSpell();

    x = 0;
    y = 0;

    LOG_INFO("Player state refreshed:");
    LOG_INFO("  HP: " + std::to_string(oldHP) + " -> " + std::to_string(heallpoints));
    LOG_INFO("  Max HP: " + std::to_string(oldMaxHP) + " -> " + std::to_string(maxHealth));
    LOG_INFO("  Enemies defeated: " + std::to_string(oldEnemiesDefeated) + " -> 0");
    LOG_INFO("  Spell count: " + std::to_string(oldSpellCount) + " -> 1");
    LOG_INFO("  Position reset to (0, 0)");
}

std::vector<uint8_t> Player::Save() const {
    std::vector<uint8_t> data(SAVE_SIZE);
    size_t offset = 0;

    memcpy(data.data() + offset, &x, sizeof(x));
    offset += sizeof(x);

    memcpy(data.data() + offset, &y, sizeof(y));
    offset += sizeof(y);

    memcpy(data.data() + offset, &heallpoints, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(data.data() + offset, &enemies_defeated, sizeof(enemies_defeated));
    offset += sizeof(enemies_defeated);

    memcpy(data.data() + offset, &attack_mode, sizeof(attack_mode));
    offset += sizeof(attack_mode);

    memcpy(data.data() + offset, &slowed, sizeof(slowed));

    LOG_DEBUG("Player save data created. Size: " + std::to_string(SAVE_SIZE) + " bytes");

    return data;
}

void Player::Load(const std::vector<uint8_t>& data) {
    if (data.size() != SAVE_SIZE) {
        LOG_ERROR("Invalid player save data size: " + std::to_string(data.size()) +
            " (expected: " + std::to_string(SAVE_SIZE) + ")");
        return;
    }

    size_t offset = 0;

    memcpy(&x, data.data() + offset, sizeof(x));
    offset += sizeof(x);

    memcpy(&y, data.data() + offset, sizeof(y));
    offset += sizeof(y);

    memcpy(&heallpoints, data.data() + offset, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(&enemies_defeated, data.data() + offset, sizeof(enemies_defeated));
    offset += sizeof(enemies_defeated);

    memcpy(&attack_mode, data.data() + offset, sizeof(attack_mode));
    offset += sizeof(attack_mode);

    memcpy(&slowed, data.data() + offset, sizeof(slowed));

    LOG_INFO("Player loaded from save:");
    LOG_INFO("  Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    LOG_INFO("  HP: " + std::to_string(heallpoints) + "/" + std::to_string(maxHealth));
    LOG_INFO("  Enemies defeated: " + std::to_string(enemies_defeated));
    LOG_INFO("  Attack mode: " + std::to_string(attack_mode));
    LOG_INFO("  Slowed: " + std::string(slowed ? "Yes" : "No"));
}

size_t Player::GetSaveSize() const {
    return SAVE_SIZE;
}

std::string Player::SaveToString() const {
    std::ostringstream ss;
    ss << Entity::SaveToString();

    ss << "enemies_defeated:" << enemies_defeated << "\n";
    ss << "attack_mode:" << attack_mode << "\n";
    ss << "slowed:" << (slowed ? "1" : "0") << "\n";
    ss << "damage:" << damage << "\n";
    ss << "maxHealth:" << maxHealth << "\n";

    ss << "=== SPELLS ===" << "\n";
    ss << hand.SaveToString();

    std::string result = ss.str();
    LOG_DEBUG("Player save string created. Length: " + std::to_string(result.length()) + " chars");

    return result;
}

void Player::LoadFromString(const std::string& data) {
    Entity::LoadFromString(data);

    std::istringstream ss(data);
    std::string line;
    bool readingSpells = false;
    std::stringstream spellsData;

    LOG_DEBUG("Loading player from string data. Length: " + std::to_string(data.length()));

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');

        if (line == "=== SPELLS ===") {
            readingSpells = true;
            LOG_DEBUG("Starting to read spells section");
            continue;
        }

        if (readingSpells) {
            spellsData << line << "\n";
        }
        else if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            if (key == "enemies_defeated") {
                enemies_defeated = std::stoi(value);
                LOG_DEBUG("Loaded enemies_defeated: " + value);
            }
            else if (key == "attack_mode") {
                attack_mode = std::stoi(value);
                LOG_DEBUG("Loaded attack_mode: " + value);
            }
            else if (key == "slowed") {
                slowed = (value == "1");
                LOG_DEBUG("Loaded slowed: " + value);
            }
            else if (key == "damage") {
                damage = std::stoi(value);
                LOG_DEBUG("Loaded damage: " + value);
            }
            else if (key == "maxHealth") {
                maxHealth = std::stoi(value);
                LOG_DEBUG("Loaded maxHealth: " + value);
            }
        }
    }

    if (readingSpells) {
        std::string spellsStr = spellsData.str();
        LOG_DEBUG("Loading spells. Data size: " + std::to_string(spellsStr.length()) + " chars");
        hand.LoadFromString(spellsStr);
        LOG_INFO("Player spells loaded successfully. Count: " + std::to_string(hand.CountSpells()));
    }

    LOG_INFO("Player loaded from string successfully");
}

std::string Player::GetSaveHeader() const {
    return "PLAYER";
}

void Player::UpgradeMaxHealth(int amount) {
    int oldMaxHealth = maxHealth;
    maxHealth += amount;
    heallpoints += amount;

    std::cout << "Max health increased to " << maxHealth << "!" << std::endl;

    LOG_INFO("Player max health upgraded: " + std::to_string(oldMaxHealth) +
        " -> " + std::to_string(maxHealth));
    LOG_INFO("Current HP: " + std::to_string(heallpoints) + "/" + std::to_string(maxHealth));

    GameEventManager::getInstance().logPlayerLevelUp(0);
}

void Player::UpgradeBaseDamage(int amount) {
    int oldDamage = damage;
    damage += amount;

    std::cout << "Base damage increased to " << damage << "!" << std::endl;

    LOG_INFO("Player base damage upgraded: " + std::to_string(oldDamage) +
        " -> " + std::to_string(damage));

    EVENT_MANAGER.logPlayerLevelUp(0); 
}

void Player::UpgradeRandomSpell() {
    int spellCount = hand.CountSpells();

    LOG_INFO("Attempting to upgrade random spell. Current spell count: " + std::to_string(spellCount));

    if (spellCount > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, spellCount - 1);

        int spellIndex = distrib(gen);

        LOG_INFO("Selected spell " + std::to_string(spellIndex + 1) + " for upgrade");

        hand.UpgradeSpellDamage(spellIndex, 10);

        std::cout << "Random spell upgraded! Damage increased by 10!" << std::endl;
        LOG_INFO("Spell " + std::to_string(spellIndex + 1) + " upgraded (+10 damage)");
    }
    else {
        std::cout << "No spells to upgrade! Adding a new spell instead." << std::endl;
        LOG_WARNING("No spells available for upgrade. Adding new spell instead.");

        hand.AddRandomSpell();
        LOG_INFO("New random spell added instead of upgrade");
    }
}

std::string Player::GetStats() const {
    std::string stats = "Player Stats:\n";
    stats += "  Health: " + std::to_string(heallpoints) + "/" + std::to_string(maxHealth) + "\n";
    stats += "  Base Damage: " + std::to_string(damage) + "\n";
    stats += "  Spells: " + std::to_string(hand.CountSpells()) + "\n";
    stats += "  Enemies Defeated: " + std::to_string(enemies_defeated) + "\n";
    stats += "  Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")\n";
    stats += "  Attack Mode: " + std::to_string(attack_mode) + "\n";
    stats += "  Slowed: " + std::string(slowed ? "Yes" : "No") + "\n";

    LOG_DEBUG("Player stats requested");

    return stats;
}

void Player::LogCurrentState() const {
    LOG_INFO("=== PLAYER STATE ===");
    LOG_INFO("Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    LOG_INFO("Health: " + std::to_string(heallpoints) + "/" + std::to_string(maxHealth));
    LOG_INFO("Damage: " + std::to_string(damage));
    LOG_INFO("Spells: " + std::to_string(hand.CountSpells()));
    LOG_INFO("Enemies defeated: " + std::to_string(enemies_defeated));
    LOG_INFO("Attack mode: " + std::to_string(attack_mode));
    LOG_INFO("Slowed: " + std::string(slowed ? "Yes" : "No"));
    LOG_INFO("====================");
}
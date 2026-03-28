#include "SavesManager.hpp"


SavesManager::SavesManager() {
}


SavesManager::~SavesManager() {
}


static int safe_stoi(const std::string& s) {
    try {
        if (s.empty()) return 0;
        return std::stoi(s);
    } catch (...) {
        return 0;
    }
}


std::string SavesManager::xorEncryptDecrypt(const std::string& text, const std::string& key) {
    std::string result = text;
    size_t key_len = key.length();
    for (size_t i = 0; i < text.length(); ++i) {
        result[i] = text[i] ^ key[i % key_len];
    }
    return result;
}


std::size_t SavesManager::getHash(const std::string& text) {
    return std::hash<std::string>{}(text);
}


bool SavesManager::isSaveCorrect(const std::string& text, std::string hash) {
    return std::to_string(getHash(text)) == hash;
}


std::string SavesManager::getCorrectSaveName(std::string saveName) {
    std::vector<std::string> savesList = getSavesList(0, -1);

    auto it = std::find(savesList.begin(), savesList.end(), saveName);
    int counter = 1;
    std::string originalName = saveName;

    FileHandler file("log_getCorrectSaveName.txt", std::ios::app);
    file.write("LOG: " + std::to_string(std::time(0)) + "\n");
    file.write("Current saveName: " + saveName + "\n");
    file.write("Other saves:\n");
    for (auto save: savesList) {
        file.write(save+'\n');
    }

    while (it != savesList.end()) {
        saveName = originalName + std::to_string(counter++);
        it = std::find(savesList.begin(), savesList.end(), saveName);
    }

    file.write("New saveName: " + saveName + "\n\n\n");
    return saveName;
}


std::string SavesManager::getStringFromEnemySaveData(std::vector<struct EnemySaveData>& enemySaveData) {
    std::string stringData(std::to_string((int)enemySaveData.size()) + '\n');
    for (auto enemyData : enemySaveData) {
        stringData += std::to_string(enemyData.enemyAttack.attack) + '\n';
        stringData += std::to_string(enemyData.enemyHealth.currentHealth) + '\n';
        stringData += std::to_string(enemyData.enemyHealth.maxHealth) + '\n';
        stringData += std::to_string(enemyData.enemyLevel) + '\n';
        stringData += std::to_string(enemyData.enemyIndex) + '\n';
    }
    return stringData;
}


std::string SavesManager::getStringFromSpellSaveData(struct SpellSaveData& spellData) {
    std::string stringData;
    stringData += std::to_string(spellData.countOfItem) + '\n';
    stringData += std::to_string(spellData.powerOfSpell) + '\n';
    stringData += std::to_string(spellData.baseDamage) + '\n';
    stringData += std::to_string(spellData.baseDistance) + '\n';
    stringData += std::to_string(spellData.trapLevel) + '\n';
    stringData += std::to_string(spellData.trapDamage) + '\n';
    return stringData;
}


std::string SavesManager::getStringFromCellsSaveData(std::vector<struct CellSaveData>& cellsData) {
    std::string stringData(std::to_string((int)cellsData.size()) + '\n');
    for (auto& cell : cellsData) {
        stringData += std::to_string(cell.index) + '\n';
        stringData += std::to_string(cell.X) + '\n';
        stringData += std::to_string(cell.Y) + '\n';
        stringData += std::to_string(cell.stateData.avaible) + '\n';
        stringData += std::to_string(cell.stateData.haveConstState) + '\n';
        stringData += std::to_string(cell.stateData.haveTempState) + '\n';
        if (cell.stateData.haveConstState) {
            stringData += std::to_string(cell.stateData.constState.durationOfState) + '\n';
            stringData += std::to_string(cell.stateData.constState.damage) + '\n';
            stringData += std::to_string(cell.stateData.constState.stateSymbol) + '\n';
        }
        else {
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
        }
        if (cell.stateData.haveTempState) {
            stringData += std::to_string(cell.stateData.tempState.durationOfState) + '\n';
            stringData += std::to_string(cell.stateData.tempState.damage) + '\n';
            stringData += std::to_string(cell.stateData.tempState.stateSymbol) + '\n';
        }
        else {
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
        }
    }
    return stringData;
}


std::string SavesManager::serializeData(SaveData& data) {
    SaveData saveData = data;
    FieldSaveData fieldSaveData = data.fieldData;
    EntitySaveData entitySaveData = fieldSaveData.entityData;
    PlayerSaveData playerSaveData = entitySaveData.playerData;
    AttackSaveData playerAttackSaveData = playerSaveData.playerAttack;
    AttributesSaveData playerAttributesSaveData = playerSaveData.playerAttributes;
    EquipmentSaveData playerEquipmentSaveData = playerSaveData.playerEquipment;
    HandSaveData playerHandSaveData = playerSaveData.playerHand;
    HealthSaveData playerHealthSaveData = playerSaveData.playerHealth;
    StatsSaveData playerStatsSaveData = playerSaveData.playerStats;
    BarrackSaveData barrackSaveData = entitySaveData.barrackData;
    HealthSaveData barrackHealthSaveData = barrackSaveData.barracksHealth;
    TowerSaveData towerSaveData = entitySaveData.towerData;
    HealthSaveData towerHealthSaveData = towerSaveData.towerHealth;
    std::string timestampData(std::to_string(std::time(0)) + '\n');
    std::string strindSaveData(
        std::to_string(saveData.gameID) + '\n' +
        std::to_string(fieldSaveData.widthField) + '\n' +
        std::to_string(fieldSaveData.heightField) + '\n' +
        std::to_string(fieldSaveData.gameLevel) + '\n' +
        std::to_string(fieldSaveData.gameTurn) + '\n' +
        std::to_string(playerAttackSaveData.attack) + '\n' +
        std::to_string(playerAttributesSaveData.intelligence) + '\n' +
        std::to_string(playerAttributesSaveData.dexterity) + '\n' +
        std::to_string(playerAttributesSaveData.strength) + '\n' +
        std::to_string(playerEquipmentSaveData.currentWeapon) + '\n' +
        std::to_string(playerEquipmentSaveData.meleeWeaponMulti) + '\n' +
        std::to_string(playerEquipmentSaveData.rangeWeaponMulti) + '\n' +
        std::to_string(playerHandSaveData.itemInHand) + '\n' +
        std::to_string(playerHandSaveData.powerUp) + '\n' +
        std::to_string(playerHandSaveData.maxSize) + '\n' +
        std::to_string(playerHandSaveData.currentSize) + '\n' +
        getStringFromSpellSaveData(playerHandSaveData.areaSpell) +
        getStringFromSpellSaveData(playerHandSaveData.directSpell) +
        getStringFromSpellSaveData(playerHandSaveData.updateSpell) +
        getStringFromSpellSaveData(playerHandSaveData.trapSpell) +
        std::to_string(playerHealthSaveData.currentHealth) + '\n' +
        std::to_string(playerHealthSaveData.maxHealth) + '\n' +
        std::to_string(playerStatsSaveData.prevLevelUpExperience) + '\n' +
        std::to_string(playerStatsSaveData.currentExperience) + '\n' +
        std::to_string(playerStatsSaveData.levelUpExperience) + '\n' +
        std::to_string(playerStatsSaveData.level) + '\n' +
        std::to_string(playerStatsSaveData.levelIncreased) + '\n' +
        std::to_string(playerSaveData.slowed) + '\n' +
        std::to_string(playerSaveData.playerIndex) + '\n' +
        getStringFromEnemySaveData(entitySaveData.enemyData) +
        std::to_string(barrackSaveData.spawnPeriod) + '\n' +
        std::to_string(barrackSaveData.barracksLevel) + '\n' +
        std::to_string(barrackSaveData.counter) + '\n' +
        std::to_string(barrackHealthSaveData.currentHealth) + '\n' +
        std::to_string(barrackHealthSaveData.maxHealth) + '\n' +
        std::to_string(barrackSaveData.barrackIndex) + '\n' +
        std::to_string(towerSaveData.attackPeriod) + '\n' +
        std::to_string(towerSaveData.attackCooldown) + '\n' +
        std::to_string(towerSaveData.towerlevel) + '\n' +
        std::to_string(towerHealthSaveData.currentHealth) + '\n' +
        std::to_string(towerHealthSaveData.maxHealth) + '\n' +
        std::to_string(towerSaveData.towerIndex) + '\n' +
        getStringFromSpellSaveData(towerSaveData.towerSpell) +
        getStringFromCellsSaveData(fieldSaveData.cellsData)
    );
    std::string hashSaveData(std::to_string(getHash(strindSaveData)) + '\n');
    return timestampData + hashSaveData + xorEncryptDecrypt(strindSaveData, XORENCDECKEY);
}


void SavesManager::newSave(SaveData& data, std::string saveName) {
    try {
        std::string fullSavePath(SAVEPATH + saveName);
        std::string correctSaveName = getCorrectSaveName(fullSavePath);
        FileHandler file(correctSaveName, std::ios::out | std::ios::binary);
        file.write(serializeData(data));
    } catch(...) {

    }
}


void SavesManager::parseSpellSaveData(std::stringstream& ss, SpellSaveData& spellData) {
    std::string line;
    std::getline(ss, line); spellData.countOfItem = safe_stoi(line);
    std::getline(ss, line); spellData.powerOfSpell = safe_stoi(line);
    std::getline(ss, line); spellData.baseDamage = safe_stoi(line);
    std::getline(ss, line); spellData.baseDistance = safe_stoi(line);
    std::getline(ss, line); spellData.trapLevel = safe_stoi(line);
    std::getline(ss, line); spellData.trapDamage = safe_stoi(line);
}

void SavesManager::parseEnemySaveData(std::stringstream& ss, std::vector<struct EnemySaveData>& enemySaveData) {
    std::string line;
    std::getline(ss, line);
    int enemyCount = safe_stoi(line);
    enemySaveData.resize(enemyCount);

    for (int i = 0; i < enemyCount; i++) {
        std::getline(ss, line); enemySaveData[i].enemyAttack.attack = safe_stoi(line);
        std::getline(ss, line); enemySaveData[i].enemyHealth.currentHealth = safe_stoi(line);
        std::getline(ss, line); enemySaveData[i].enemyHealth.maxHealth = safe_stoi(line);
        std::getline(ss, line); enemySaveData[i].enemyLevel = safe_stoi(line);
        std::getline(ss, line); enemySaveData[i].enemyIndex = safe_stoi(line);
    }
}

void SavesManager::parseCellsSaveData(std::stringstream& ss, std::vector<struct CellSaveData>& cellsData) {
    std::string line;
    std::getline(ss, line);
    int cellCount = safe_stoi(line);
    cellsData.resize(cellCount);

    for (int i = 0; i < cellCount; i++) {
        std::getline(ss, line); cellsData[i].index = safe_stoi(line);
        std::getline(ss, line); cellsData[i].X = safe_stoi(line);
        std::getline(ss, line); cellsData[i].Y = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.avaible = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.haveConstState = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.haveTempState = safe_stoi(line);

        std::getline(ss, line); cellsData[i].stateData.constState.durationOfState = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.constState.damage = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.constState.stateSymbol = safe_stoi(line);

        std::getline(ss, line); cellsData[i].stateData.tempState.durationOfState = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.tempState.damage = safe_stoi(line);
        std::getline(ss, line); cellsData[i].stateData.tempState.stateSymbol = safe_stoi(line);
    }
}


SaveData SavesManager::deserializeData(std::string& serializedData) {
    SaveData saveData;
    std::stringstream ss(serializedData);
    std::string line;

    std::getline(ss, line);
    saveData.gameID = safe_stoi(line);

    std::getline(ss, line);
    saveData.fieldData.widthField = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.heightField = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.gameLevel = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.gameTurn = safe_stoi(line);

    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerAttack.attack = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerAttributes.intelligence = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerAttributes.dexterity = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerAttributes.strength = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerEquipment.currentWeapon = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerEquipment.meleeWeaponMulti = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerEquipment.rangeWeaponMulti = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHand.itemInHand = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHand.powerUp = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHand.maxSize = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHand.currentSize = safe_stoi(line);

    parseSpellSaveData(ss, saveData.fieldData.entityData.playerData.playerHand.areaSpell);
    parseSpellSaveData(ss, saveData.fieldData.entityData.playerData.playerHand.directSpell);
    parseSpellSaveData(ss, saveData.fieldData.entityData.playerData.playerHand.updateSpell);
    parseSpellSaveData(ss, saveData.fieldData.entityData.playerData.playerHand.trapSpell);

    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHealth.currentHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerHealth.maxHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerStats.prevLevelUpExperience = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerStats.currentExperience = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerStats.levelUpExperience = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerStats.level = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerStats.levelIncreased = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.slowed = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.playerData.playerIndex = safe_stoi(line);

    parseEnemySaveData(ss, saveData.fieldData.entityData.enemyData);

    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.spawnPeriod = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.barracksLevel = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.counter = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.barracksHealth.currentHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.barracksHealth.maxHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.barrackData.barrackIndex = safe_stoi(line);

    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.attackPeriod = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.attackCooldown = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.towerlevel = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.towerHealth.currentHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.towerHealth.maxHealth = safe_stoi(line);
    std::getline(ss, line);
    saveData.fieldData.entityData.towerData.towerIndex = safe_stoi(line);

    parseSpellSaveData(ss, saveData.fieldData.entityData.towerData.towerSpell);
    parseCellsSaveData(ss, saveData.fieldData.cellsData);
    return saveData;
}

SaveData SavesManager::getLoadGameData(std::string saveName) {
    try {
        std::string fullSavePath = SAVEPATH + saveName;
        FileHandler file(fullSavePath, std::ios::in | std::ios::binary);
        std::istream& is = file.stream();
        if (!is) {
            throw std::runtime_error("Failed to open file");
        }
        std::string serializedData((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

        if (serializedData.empty()) {
            throw std::runtime_error("Saving contains no data");
        }

        std::stringstream ss(serializedData);
        std::string timestamp, hash, encryptedData;

        if (!std::getline(ss, timestamp)) {
            throw std::runtime_error("Missing timestamp");
        }
        if (!std::getline(ss, hash)) {
            throw std::runtime_error("Missing hash");
        }
        encryptedData.assign(std::istreambuf_iterator<char>(ss), std::istreambuf_iterator<char>());
        std::string decryptedData = xorEncryptDecrypt(encryptedData, XORENCDECKEY);
        if (!isSaveCorrect(decryptedData, hash)) {
            throw std::runtime_error("The file is damaged (hash mismatch)");
        }
        return deserializeData(decryptedData);
    }
    catch(const std::exception& e) {
        throw;
    }
}


std::vector<std::string> SavesManager::getSavesList(int start, int count) {
    namespace fs = std::filesystem;
    std::vector<std::tuple<std::string, std::string, fs::file_time_type>> savesData;
    try {
        for (const auto& entry : fs::directory_iterator(SAVEPATH)) {
            try {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    std::string key = entry.path().filename().string();
                    auto lastWrite = entry.last_write_time();
                    std::ifstream file(filePath);
                    if (file) {
                        std::string firstLine;
                        if (std::getline(file, firstLine)) {
                            if (!firstLine.empty()) {
                                key = std::move(firstLine);
                            }
                        }
                    }
                    savesData.emplace_back(std::move(key), std::move(filePath), lastWrite);
                }
            } catch (const std::exception& e) {
                continue;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
        return {};
    }
    std::sort(savesData.begin(), savesData.end(),
              [](const auto& a, const auto& b) {
                  return std::get<0>(a) > std::get<0>(b);
              });

    start = std::max(0, start);
    int end = (count == -1) ? static_cast<int>(savesData.size()) :
                              std::min(start + count, static_cast<int>(savesData.size()));
    if (start >= end) {
        return {};
    }
    std::vector<std::string> savesList;
    savesList.reserve(end - start);
    for (int i = start; i < end; ++i) {
        savesList.push_back(std::get<1>(savesData[i]));
    }
    return savesList;
}
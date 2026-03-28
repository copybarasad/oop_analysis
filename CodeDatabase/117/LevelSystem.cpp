#include "LevelSystem.h"

#include "FileWrapper.h"
#include "../SaveLoadWrappers/GameStateSaveWrapper.h"
#include "../Exceptions/SaveLoadException.h"


#include "../LogSystem/Notifier.h"
#include "../LogSystem/NewGameEvent.h"
#include "../LogSystem/LoadEvent.h"
#include "../LogSystem/SaveEvent.h"


LevelSystem::LevelSystem(){
    this->levelNum = 1;
}

void LevelSystem::save(std::string saveFileName, GameState gameState){
    saveFileName += ".json";
    nlohmann::json output;
    
    GameStateSaveWrapper gameStateSaveWrapper(gameState);
    try{
        gameStateSaveWrapper.save(output);
    }
    catch(std::exception& e){
        throw SaveException(e.what());
    }
    
    output["LevelNum"] = this->levelNum;
    output["LevelName"] = this->levelTitle;
    output["Hash"] = this->getHash(gameState);
    
    FileWrapper file = FileWrapper(saveFileName, Mode::Write);
    file.write(output);
}

void LevelSystem::load(std::string saveFileName, GameState& gameState){
    saveFileName += ".json";
    FileWrapper file = FileWrapper(saveFileName, Mode::Read);
    nlohmann::json input = file.read();
    try{
        this->levelNum = input["LevelNum"];
        this->levelTitle = input["LevelName"];
        GameState newGameState = GameState(Field(), Hand(), Player());
        GameStateSaveWrapper gameStateSaveWrapper(newGameState);
        gameStateSaveWrapper.load(input);
        // std::cout << this->getHash(gameState) << ' ' << input["Hash"] << std::endl;
        if (this->getHash(newGameState) != input["Hash"]){
            throw ValidationException(saveFileName);
        }
        gameState = newGameState;
    }
    catch(nlohmann::json::type_error& e){
        throw CorruptedSaveException(saveFileName, e.what());
    }
    catch(nlohmann::json::parse_error& e){
        throw CorruptedSaveException(saveFileName, e.what());
    }
    catch(std::exception& e){
        throw LoadException(e.what());
    }
}

void LevelSystem::loadSave(GameState& gameState, int slotNum){
    if (slotNum > MAX_SLOT_NUM || slotNum < 1){
        throw LoadException("file slot with number " + std::to_string(slotNum) + "does not exist");
    }
    this->load(SAVES_PATH + "/" + "Save Slot " + std::to_string(slotNum), gameState);
    this->save(SAVE_LEVEL_PATH, gameState);

    LoadEvent event = LoadEvent(this->levelTitle, slotNum, false);
    Notifier::getInstance().notify(event);
}

void LevelSystem::saveToSlot(GameState& gameState, int slotNum){
    if (slotNum > MAX_SLOT_NUM || slotNum < 1){
        throw LoadException("file slot with number " + std::to_string(slotNum) + " does not exist");
    }
    this->save(SAVES_PATH + "/" + "Save Slot " + std::to_string(slotNum), gameState);
    SaveEvent event = SaveEvent(slotNum);
    Notifier::getInstance().notify(event);
}

void LevelSystem::loadNewGame(GameState& gameState){
    this->load(LEVEL_PATH + "/" + "Level 1", gameState);
    gameState.getHand().addRandomSpell();
    this->save(SAVE_LEVEL_PATH, gameState);
    NewGameEvent event = NewGameEvent();
    Notifier::getInstance().notify(event);
}

int LevelSystem::loadNextLevel(GameState& gameState, UpgradeType type){
    this->levelNum += 1;
    if (this->levelNum > MAX_LEVEL_NUM){
        return -1;
    }

    Player player = gameState.getPlayer();
    int health = player.getMaxHP();

    int meleeDMG = 0;
    int rangeDMG = 0;
    if (player.getDMGMode() == DamageMode::Melee){
        meleeDMG = player.getDMG();
        player.switchDMGMode();
        rangeDMG = player.getDMG();
    }
    else{
        rangeDMG = player.getDMG();
        player.switchDMGMode();
        meleeDMG = player.getDMG();
    }

    int expoints = player.getEXP();

    Hand oldHand = gameState.getHand();
    int handCount = oldHand.getCount();
    Hand hand = Hand(oldHand.getSize());
    hand.setUpgradePower(oldHand.getUpgradePower());
    for(int i = handCount; i > handCount/ 2; --i){
        int randNum = rand() % i;
        hand.addSpell(oldHand.getSpell(randNum));
        oldHand.removeSpell(randNum);
    }

    this->load(LEVEL_PATH + "/" + "Level " + std::to_string(levelNum), gameState);

    switch (type)
    {
    case UpgradeType::HealthUpgrade:
        gameState.getPlayer().setMaxHP(health + expoints);
        gameState.getPlayer().setHP(health + expoints);
        break;
    
    case UpgradeType::RangeUpgrade:
        if (gameState.getPlayer().getDMGMode() == DamageMode::Melee){
            gameState.getPlayer().switchDMGMode();
        }
        gameState.getPlayer().setDMG(rangeDMG + expoints / 5);
        break;
    
    case UpgradeType::MeleeUpgrade:
        if (gameState.getPlayer().getDMGMode() == DamageMode::Range){
            gameState.getPlayer().switchDMGMode();
        }
        gameState.getPlayer().setDMG(meleeDMG + expoints / 5);
        break;

    case UpgradeType::SpellUpgrade:
        hand.setUpgradePower(hand.getUpgradePower() + expoints / 5);
        break;

    default:
        break;
    }

    gameState.setHand(hand);

    this->save(SAVE_LEVEL_PATH, gameState);

    LoadEvent event = LoadEvent(this->levelTitle, 0, true);
    Notifier::getInstance().notify(event);

    return 0;
}

void LevelSystem::restartLevel(GameState& gameState){
    this->load(SAVE_LEVEL_PATH, gameState);
}

std::string LevelSystem::getLevelName() const{
    return this->levelTitle;
}

int LevelSystem::getLevelNum() const{
    return this->levelNum;
}

void LevelSystem::setLevelNum(int newLevelNum){
    this->levelNum = std::max(1, newLevelNum);
}

size_t LevelSystem::getHash(GameState& gameState) const {
    GameStateSaveWrapper wrapper(gameState);
    size_t hash = wrapper.getHash();
    std::hash<int> intHash;
    std::hash<std::string> stringHash;
    hash += intHash(this->levelNum);
    hash += stringHash(this->levelTitle);
    return hash;
}
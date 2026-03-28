#include "managment/game.h"
#include "controllers/command.h"
#include "controllers/IGameView.h"
#include "controllers/IInputHandler.h"

#define FALL Position{-1, -1}

Game::Game()
{
    level_ = 0;
    player_ = std::make_unique<Player>(gameConf.playerHealth, gameConf.playerDamage, gameConf.playerStartPos);
    field_ = Field(gameConf.fieldSize);
    field_.addPlayer(player_->getPosition());
    enemyManager_ = EnemyManager(field_);
    spellManager_ = SpellManager();
    isRunning_ = true;
}

void Game::improvePlayer(char opt, IGameView &view, IInputHandler &inputHandler)
{
    std::unique_ptr<ImproveSpell> spell;
    int idx;
    Command idxCmd;

    switch (opt)
    {
    case 'H':
        gameConf.playerHealth += 20;
        player_->setHealth(gameConf.playerHealth);
        break;
    case 'D':
        gameConf.playerDamage += 10;
        player_->setDamage(gameConf.playerDamage);
        break;
    case 'S':
        spell = std::make_unique<ImproveSpell>();
        view.drawHand(*player_->getHand());
        idxCmd = inputHandler.selectIndex(player_->getHand()->size() + 1);
        if (!idxCmd.index.has_value() || idxCmd.index.value() == 0)
            break;
        idx = idxCmd.index.value();
        spellManager_.cast(*spell, field_, enemyManager_, player_->getHand(), idx);
        break;
    default:
        break;
    }
}

void Game::newLevel(char improvementOpt, IGameView &view, IInputHandler &inputHandler)
{
    int spSize = player_->getHand()->size();
    for (int i = spSize - 1; i >= spSize / 2; i--)
    {
        player_->getHand()->deliteSpell(i);
    }
    
    this->improvePlayer(improvementOpt, view, inputHandler);
    enemyManager_.improveEnemies();

    enemyManager_.cleanEnemies();
    spellManager_.cleanAlly();

    player_->move(gameConf.playerStartPos);


    field_ = (gameConf.fieldSize < 25) ? Field(gameConf.fieldSize += 5) : Field(gameConf.fieldSize);
    field_.addPlayer(player_->getPosition());
    enemyManager_.generateEnemies(field_);

    level_++;
}


void Game::handleCommand(const Command &cmd, IGameView &view, IInputHandler &inputHandler)
{
    Position newPos = player_->getPosition();
    int attackRadius = (player_->getWeapon() == "knight") ? 1 : 4;
    Position enemyPos = FALL;
    std::unique_ptr<ISpellCard> spell;
    Command idxCmd;
    Command idxEn;
    ISpellCard *peek;
    int idx = 0;
    int enx = 0;
    Position center = {0, 0};
    std::string filename;
    std::string folderPath;

    switch (cmd.type)
    {
    case CommandType::MoveUp:
        newPos.y--;
        break;
    case CommandType::MoveDown:
        newPos.y++;
        break;
    case CommandType::MoveLeft:
        newPos.x--;
        break;
    case CommandType::MoveRight:
        newPos.x++;
        break;

    case CommandType::Attack:
        attack(player_->getDamage(), attackRadius);
        break;

    case CommandType::ChangeWeapon:
        player_->changeWeapon();
        break;

    case CommandType::OpenHand:
        view.drawHand(*player_->getHand());
        idxCmd = inputHandler.selectIndex(player_->getHand()->size()+1);
        if (!idxCmd.index.has_value() || idxCmd.index.value() == 0)
            break; 
        idx = idxCmd.index.value();
        peek = player_->getHand()->peekSpell(idx);
        if (!peek)
            break;

        if (auto *as = dynamic_cast<AreaSpell *>(peek))
        {
            Command posCmd = inputHandler.selectPosition(field_, as->getRadius(), view);
            if (!posCmd.position.has_value())
            { 
                break;
            }
            center = posCmd.position.value();

            field_.markRegion(center, as->getRadius(), true);
            view.update(*this);

            auto spellPtr = player_->getHand()->takeSpell(idx);
            spellManager_.cast(*static_cast<AreaSpell *>(spellPtr.get()), field_, enemyManager_, center);
            receiveSpell(spellPtr->name());

            field_.clearReady();
        }
        else if (auto *ds = dynamic_cast<DirectSpell *>(peek))
        {
            Position playerPos = field_.getPlayer();
            auto enemies = field_.enHInRadius(playerPos, ds->getRadius());

            view.drawEnemies(enemies);
            if (enemies.empty())
                break;

            idxEn = inputHandler.selectIndex(enemies.size());
            if (!idxEn.index.has_value())
                break;
            enx = idxEn.index.value();

            auto spellPtr = player_->getHand()->takeSpell(idx);
            spellManager_.cast(*static_cast<DirectSpell *>(spellPtr.get()), field_, enemyManager_, enemies[enx]);
            receiveSpell(spellPtr->name());
        }

        else if (auto *is = dynamic_cast<ImproveSpell *>(peek)){
            view.drawHand(*player_->getHand());
            idxEn = inputHandler.selectIndex(player_->getHand()->size() + 1);
            if (!idxEn.index.has_value() || idxEn.index.value() == 0)
                break;
            enx = idxEn.index.value();

            auto spellPtr = player_->getHand()->takeSpell(idx);
            spellManager_.cast(*static_cast<ImproveSpell *>(spellPtr.get()), field_, enemyManager_, player_->getHand(), enx);
            receiveSpell(spellPtr->name());
        }
        else if (auto *ts = dynamic_cast<TrapSpell *>(peek))
        {
            auto spellPtr = player_->getHand()->takeSpell(idx);
            spellManager_.cast(*static_cast<TrapSpell *>(spellPtr.get()), field_, enemyManager_, player_->getPosition());
            receiveSpell(spellPtr->name());
        }
        else if (auto *cs = dynamic_cast<CallSpell *>(peek))
        {
            auto spellPtr = player_->getHand()->takeSpell(idx);
            spellManager_.cast(*static_cast<CallSpell *>(spellPtr.get()), field_, enemyManager_, player_->getPosition());
            receiveSpell(spellPtr->name());
        }
        break;

    case CommandType::Save:
        
        view.drawSave();
        filename = inputHandler.readString().text.value();
        saveManager_.save(filename, spellManager_, enemyManager_, field_, *player_, level_);
        break;

    case CommandType::Load:
        
        folderPath = selectLoad(view, inputHandler);

        saveManager_.load(folderPath, spellManager_, enemyManager_, field_, *player_, level_);
        break;

    case CommandType::Quit:
        isRunning_ = false;
        break;

    default:
        break;
    }

    movePlayer(newPos);
}

void Game::update(Command upOpt, IGameView &view, IInputHandler &inputHandler)
{
    if (!player_->isAlive())
    {
        isRunning_ = false;
        return;
    }

    if (upOpt.type == CommandType::LevelUp)
    {
        view.drawImproveOptions();
        Command up = inputHandler.readOptions();
        char improvementOption = up.opt.value();
        this->newLevel(improvementOption, view, inputHandler);
    }

    enemyManager_.enemyMove(*player_, field_, spellManager_);
    enemyManager_.enemyHutsMove(*player_, field_);
    enemyManager_.towerMove(*player_, field_);
    spellManager_.allyMove(field_, enemyManager_);
}

bool Game::levelUp(){
    return enemyManager_.enemyEmpty();
}

bool Game::isRunning() const
{
    return isRunning_;
}

Player *Game::getPlayer() const
{
    return player_.get();
}

Field &Game::getField() { return field_; }

const Field &Game::getField() const { return field_; }

EnemyManager &Game::getEnemyManager() { return enemyManager_; }

const EnemyManager &Game::getEnemyManager() const { return enemyManager_; }

SpellManager &Game::getSpellManager()
{
    return spellManager_;
}

int Game::getLevel() const
{
    return level_;
}

std::string Game::selectLoad(IGameView &view, IInputHandler &inputHandler)
{
    namespace fs = std::filesystem;
    fs::path folderPath = "../save";
    view.drawSaveDir(folderPath);
    std::vector<std::string> saves;

    int i = 0;
    for (const auto &entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            saves.push_back(filePath);
        }
    }
    if (saves.empty())
    {
        return "";
    }

    int index = inputHandler.selectIndex(saves.size()).index.value();

    return saves[index];
}

void Game::loadGame(IGameView &view, IInputHandler &inputHandler)
{
    std::string folderPath = selectLoad(view, inputHandler);
    saveManager_.load(folderPath, spellManager_, enemyManager_, field_, *player_, this->level_ );
}

void Game::setLogger(std::shared_ptr<ILogger> logger){
    eventManager_.addLogger(logger);
}
void Game::movePlayer(Position newPos){

    if (!player_->thisIsSlow())
    {
        if (field_.isFree(newPos))
        {
            field_.deliteCharacter(player_->getPosition());
            player_->move(newPos);
            field_.addPlayer(newPos);
            if (field_.isSlow(newPos))
                player_->setIsSlow(true);
        }
    }
    else
    {
        player_->setIsSlow(!player_->thisIsSlow());
    }

    Event event{EventType::PlayerMoved, "Player moved to (" + std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + ")", newPos};
    eventManager_.notify(event);
}
void Game::attack(int damage, int attackRadius){

    Position enemyPos = field_.enemyInRadius(player_->getPosition(), attackRadius);
    enemyManager_.setDamage(enemyPos, player_->getDamage());

    Event event{EventType::PlayerAttacked, "Player dealt " + std::to_string(damage) + " damage", player_->getPosition(), damage, "Player"};
    eventManager_.notify(event);
}
void Game::receiveSpell(const std::string &spellName){
    Event event{EventType::PlayerReceivedSpell, "Player received spell: " + spellName, player_->getPosition(), 0, "Player"};
    eventManager_.notify(event);
}
#include "player_manager.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"

PlayerManager::PlayerManager(Player* newPlayer, std::pair<int, int> pos) : player(newPlayer), playerPosition({0, 0}) {
    spellHand = new SpellHand();
    if (!player) {
        spellHand = new SpellHand();
        int randomSpell = rand() % 3;
        
        switch(randomSpell) {
            case 0:
                spellHand->addSpell(new DirectDamageSpell(50, 1));
                break;
            case 1:
                spellHand->addSpell(new AreaDamageSpell(50, 1));
                break;
            case 2:
                spellHand->addSpell(new TrapSpell(50, 1));
                break;
        }
    }else{
        setPlayer(newPlayer, pos.first, pos.second);
    }
}

PlayerManager::~PlayerManager() {
    delete player;
    delete spellHand;
}

void PlayerManager::setPlayer(Player* newPlayer, int x, int y) {
    if (!player) delete player;
    player = newPlayer;
    playerPosition = std::pair{x, y};
}

void PlayerManager::addPlayerSpell(ISpell * spell){
    if (!player) return;
    if (spellHand->getMaxSize() == spellHand->getSpells().size()){
        std::cout << "Inventory is fully, the spell will not be added \n";
        return;
    }
    spellHand->addSpell(spell);
    player->addPoints(spell->getCost());
}

Player* PlayerManager::getPlayer() const {
    return player;
}

SpellHand& PlayerManager::getSpellHandRef() {
    return *spellHand;
}

int PlayerManager::getPlayerMaxHealth() const {
    if (player) return player->getMaxHealth();
    return 0;
}

void PlayerManager::healPlayer(int amount){
    if (!player) return;
    player->heal(amount);
}

int PlayerManager::getPlayerHealth() const {
    return player ? player->getHealth() : 0;
}

std::pair<int, int> PlayerManager::getPlayerPosition() const {
    return playerPosition;
}

void PlayerManager::takePlayerDamage(int damage) {
    if (player) {
        player->takeDamage(damage);
    }
}

void PlayerManager::upgradePlayerRandomSpell(){
    auto spells = spellHand->getSpells();

    if (spells.empty()) {
        std::cout << "No spells to upgrade!" << std::endl;
        return;
    }
        int randomIndex = rand() % spells.size();
    ISpell* spell = spellHand->getSpell(randomIndex);
    
    spell->upgrade();

    std::cout << spell->getName() << " upgraded!" << std::endl;
}

void PlayerManager::switchPlayerFightType() {
    if (player) {
        player->switchFightType();
    }
}

bool PlayerManager::getPlayerFightType() const {
    return player ? player->getFightType() : false;
}

int PlayerManager::getPlayerDamage() const {
    return player ? player->getDamage() : 0;
}

void PlayerManager::awardPoints(int points) { 
    if (player) player->addPoints(points);
}

void PlayerManager::setPlayerMaxHealth(int health){
    if (!player) return;
    player->setMaxHealth(health);
    player->heal(health);
}

void PlayerManager::setPlayerDamage(int damage){
    if (!player) return;
    player->setDamage(damage);
}

void PlayerManager::movePlayer(int x, int y) {
    playerPosition = std::pair{x, y};
}

bool PlayerManager::hasPlayerAt(int x, int y){
    return playerPosition.first == x && playerPosition.second == y;
}

SpellHand* PlayerManager::getSpellHand() const {
    return spellHand;
}

int PlayerManager::getPlayerPoints() const {
    return player->getPoints();
}
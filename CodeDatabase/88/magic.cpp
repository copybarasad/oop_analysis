#include "magic.hpp"

MagicHand::MagicHand(GameMap& map, int capacity)
        : capacity(capacity),
          map(map),
          movement(map),
          combat(map),
          spellRange(3)
          {
    getRandSpell();
}

void MagicHand::getRandSpell(){
    const char spells[6] = {'t', 'b', 's', 'n', 'u', 'i'};
    int idx = std::rand() % 6;
    if(inventory.size() < static_cast<size_t>(capacity)){
        inventory.push_back(spells[idx]);
        std::cout << "You have found a new spell!\n"; 
    }
}
void MagicHand::setInventory(json& newInventory){
    std::vector<char> newInv; 
    for(auto& i: newInventory){
        newInv.push_back(i.get<char>());
    }
    inventory = newInv;
}

void MagicHand::punish(){
    std::cout << "What? Didn't quite catch that!" << "\n";
    std::cout << "Your spell backfired! You recieve 5 damage!" << "\n";
    player->takeDamage(5);
        
}

void MagicHand::getArgs(int& dx, int& dy, int& dist, bool fg){
    std::string input;
    std::getline(std::cin, input);

    std::string direction;
    std::istringstream iss(input);
    
    iss >> direction;
    if(fg) iss >> dist;
    movement.getDirectionOffset(direction, dx, dy);
    
}

void MagicHand::cast(char spell){
    if (spell == 'i') {
        improve();
        return;
    }

    int stacks = improvementStacks;
    improvementStacks = 0;
    int dx = 0;
    int dy = 0;
    int dist = 0;
    switch (spell) {
        case 't': {
            std::cout << "Where shall you release your zapping power: Up(w), Right(d), Left(a) or Down(s)? How many tiles away from you?\n";
            int oldRange = spellRange;
            spellRange = spellRange + stacks;
            getArgs(dx, dy, dist, true);
            thunderbolt(dx, dy, dist);
            spellRange = oldRange;
            break;
        }
        case 'b': { 
            std::cout << "Where shall you release your surging power: Up(w), Right(d), Left(a) or Down(s)?\n";
            int oldRange = spellRange;
            spellRange = spellRange + stacks;
            getArgs(dx, dy, dist, false);
            brimstone(dx, dy);
            spellRange = oldRange;
            break;
        }
        case 's': { 
            std::cout << "Where shall you unleash the power of the Sun: Up(w), Right(d), Left(a) or Down(s)? Command how many yards(tiles) away from you.\n";
            improvementStacks = stacks;
            getArgs(dx, dy, dist, true);
            sunstrike(dx, dy, dist);
            improvementStacks = 0;
            break;
        }
        case 'n': { 
            std::cout << "Where shall you unleash the power of trickery: Up(w), Right(d), Left(a) or Down(s)? Command how many yards(tiles) away from you.\n";
            int oldTrap = map.getTrapDamage();
            map.setTrapDamage(oldTrap +10 + 10 * stacks);
            getArgs(dx, dy, dist, true);
            snare(dx, dy, dist);
            map.setTrapDamage(oldTrap);
            break;
        }
        case 'u': {
            int count = 1 + stacks;
            for (int i = 0; i < count; ++i) {
                summon();
            }
            break;
        }
        default:
            punish();
            break;
    }
}

void MagicHand::handleInput(const std::string& command){
    std::istringstream iss(command);
    std::string keyword;
    size_t index;
    iss >> keyword >> index;

    if (index >= inventory.size()) {
        std::cout << "Your greed sickens me.\n";
        return;
    }
    char spell = inventory[index];
    inventory.erase(inventory.begin() + static_cast<long long>(index));
    cast(spell);
    
}

void MagicHand::displayHand() const {
    std::cout << "Spells: ";
    if (inventory.empty()) {
        std::cout << "empty\n";
        return;
    }
    for (size_t i = 0; i < inventory.size(); ++i) {
        char c = inventory[i];
        std::string name;
        if (c == 't') name = "Thunderbolt";
        else if (c == 'b') name = "Brimstone";
        else if (c == 's') name = "Sunstrike";
        else if (c == 'n') name = "Snare";
        else if (c == 'u') name = "Summon";
        else if (c == 'i') name = "Improve";
        std::cout << i << ": " << name << "/ ";
    }
    
    std::cout << "\n";
}


void MagicHand::thunderbolt(int dx, int dy, int dist, Character* mage, Player* player1){
    Character* target;
    int x, y;
    int mod = 0;
    if(!mage){
        if(dx == dy){
            punish();
            return;
        }
        x = player->getX();
        y = player->getY();
        dist = std::min(spellRange, dist);
        target = map.getCharacterAt(x + dx*dist, y + dy*dist);
    } else {
        target = player1;
        std::cout << target->getIcon() << "\n";
        x = mage->getX();
        y = mage->getY();
        int mod = (dx + dy) / dist; 
    }
    if(!target){
        std::cout << "You appear to have missed! Yikes...\n";
        return;
    }
    for(int i = 0; i < dist; i++){
        if(!mage && !map.isPositionPassable(x + dx*i, y + dy*i))return;
        else if(mage && !map.isPositionPassable(x + i*mod, y + i*mod)) return;
    }
    int damage_took = mage ? spellDamage/2 : spellDamage;
    
    target->takeDamage(damage_took);
    std::cout <<(mage? mage->getIcon(): player->getIcon()) <<": Zapped target! " << damage_took << " damage dealt!\n";
    if(!target->isAlive()){
        combat.handleDefeatedEnemy(target);
    }     
    
}

void MagicHand::brimstone(int dx, int dy){
    if(dx == dy){
        punish();
        return;
    }
    int x = player->getX();
    int y = player->getY();
    
    for(int i = 0; i < spellRange; i++){
        x += dx;
        y += dy;
        if(!map.isPositionPassable(x, y)) break;
        Character* character;
        if((character = map.getCharacterAt(x, y))){
            character->takeDamage(spellDamage);
            std::cout << "P: Melted target! " << spellDamage << " damage dealt!\n";
            if(!character->isAlive()){
                combat.handleDefeatedEnemy(character);
            } 
        }


    }
}

void MagicHand::sunstrike(int dx, int dy, int dist){
    if(dx == dy){
        punish();
        return;
    }
    
    int x = player->getX();
    int y = player->getY();
    Character* character;
    int size = 2 + improvementStacks;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int t_x = x + dx * dist + j;
            int t_y = y + dy * dist + i;
                     
            if((character = map.getCharacterAt(t_x, t_y))){
                character->takeDamage(10);
                std::cout << "P: Scorched target! " << 10 << " damage dealt! Target is set ablaze!\n";
                character->setAblaze();
            if(!character->isAlive()){
                combat.handleDefeatedEnemy(character);
            } 
        }
        }
    }
}

void MagicHand::snare(int dx, int dy, int dist){
    dist = std::min(5, dist);
    if(dx == dy){
        punish();
        return;
    }
    int x = player->getX();
    int y = player->getY();
    map.setTrap(x + dx*dist, y + dist*dy);
}

void MagicHand::summon(){
    int x = 0;
    int y = 0;
    map.findSpawnPositionNearEntity(player->getX(), player->getY(), x, y);
    if(x == -1 || y == -1){
        std::cout << "Cease concealing yourself! There is no space around thee!\n";
        return;
    }
    auto spirit = std::make_unique<Ally>(10, 10, x, y);
    map.placeCharacter(spirit.get(), x, y);
    allCharacters->push_back(std::move(spirit));
    std::cout << "Spirits of the Forge have been unleashed! They are at your service.\n";
    
}

void MagicHand::improve(){
    improvementStacks += 1;
    std::cout << "Power gathers. Your might is increased: " << improvementStacks << "\n";
}
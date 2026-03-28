#include "spells/hand.h"
#include "spells/areaSpell.h"
#include "spells/directSpell.h"
#include "spells/callSpell.h"
#include "spells/improveSpell.h"
#include "spells/trapSpell.h"
#include "configs/areaConf.h"
#include "configs/directConf.h"

Hand::Hand(int size) : size_(size)
{
    generateRandomSpell();
}

int Hand::size(){return spells_.size();}


void Hand::addSpell(std::unique_ptr<ISpellCard> spell)
{
    spells_.push_back(std::move(spell));
}

void Hand::deliteSpell(int index)
{
    if (index >= 0 && index < static_cast<int>(spells_.size()))
    {
        spells_.erase(spells_.begin() + index);
    }
}


void Hand::generateRandomSpell()
{
    if (size_ <= spells_.size()){return;}

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);

    std::random_device rand;
    std::mt19937 genem(rand());
    std::uniform_int_distribution<> distruct(0, 2);

    directConf dir_conf;
    areaConf ar_conf;

    int type = dist(gen);

    if (type == 0)
    {
        int directSpell = distruct(genem);
        spells_.push_back(std::make_unique<DirectSpell>(dir_conf.directNames[directSpell], dir_conf.directDescriptions[directSpell], dir_conf.directDamage[directSpell], dir_conf.directRadius[directSpell]));
    }
    else if (type == 1)
    {
        int areaSpell = distruct(genem);
        spells_.push_back(std::make_unique<AreaSpell>(ar_conf.areaNames[areaSpell], ar_conf.areaDescriptions[areaSpell], ar_conf.areaDamage[areaSpell], ar_conf.areaRadius[areaSpell]));
    }

    else if (type == 2)
    {
        spells_.push_back(std::make_unique<ImproveSpell>());
    }
    else if (type == 3)
    {
        spells_.push_back(std::make_unique<TrapSpell>(10, dir_conf.trapPos));
    }
    else if (type == 4)
    {
        spells_.push_back(std::make_unique<CallSpell>());
    }
}

std::unique_ptr<ISpellCard> Hand::chooseSpell()
{
    std::cout << "Выбери заклинание: ";
    int sp;
    std::cin >> sp;

    if (sp < 1 || sp > static_cast<int>(spells_.size()))
    {
        if (sp == 0)
        {
            return nullptr;
        }
        std::cout << "Неверный выбор!" << std::endl;
        return nullptr;
    }

    std::unique_ptr<ISpellCard> spell = std::move(spells_[sp - 1]);
    this->deliteSpell(sp-1);
    return spell;
}



void Hand::cleanHand(){
    spells_.clear();
}

void Hand::addSpell(std::string name){
    directConf dir_conf;
    areaConf ar_conf;

    for (int sp = 0; sp < ar_conf.areaNames.size(); sp++){
        if (ar_conf.areaNames[sp] == name){
            std::unique_ptr<AreaSpell> spell = std::make_unique<AreaSpell>(name, ar_conf.areaDescriptions[sp], ar_conf.areaDamage[sp], ar_conf.areaRadius[sp]);
            spells_.push_back(std::move(spell));
            return;
        }
    }

    for (int sp = 0; sp < dir_conf.directNames.size(); sp++)
    {
        if (dir_conf.directNames[sp] == name)
        {
            std::unique_ptr<DirectSpell> spell = std::make_unique<DirectSpell>(name, dir_conf.directDescriptions[sp], dir_conf.directDamage[sp], dir_conf.directRadius[sp]);
            spells_.push_back(std::move(spell));
            return;
        }
    }


    if ("Улудшение" == name)
    {
        std::unique_ptr<ImproveSpell> spell = std::make_unique<ImproveSpell>();
        spells_.push_back(std::move(spell));
        return;
    }

    if ("Ловушка" == name)
    {
        std::unique_ptr<TrapSpell> spell = std::make_unique<TrapSpell>(10, dir_conf.trapPos);
        spells_.push_back(std::move(spell));
        return;
    }

    if ("Заклинание призыва" == name)
    {
        std::unique_ptr<CallSpell> spell = std::make_unique<CallSpell>();
        spells_.push_back(std::move(spell));
        return;
    }
}

const std::vector<std::unique_ptr<ISpellCard>>& Hand::getSpells() const{
    return spells_;
}


ISpellCard *Hand::peekSpell(int index)
{
    if (index < 1 || index > static_cast<int>(spells_.size()))  
        return nullptr;

    ISpellCard* spell = spells_[index - 1].get();  
    return spell;
}

std::unique_ptr<ISpellCard> Hand::takeSpell(int index)
{
    if (index < 1 || index > static_cast<int>(spells_.size())) 
        return nullptr;
    auto ptr = std::move(spells_[index - 1]); 
    spells_.erase(spells_.begin() + (index - 1));
    return ptr;
}

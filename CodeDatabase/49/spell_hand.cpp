#include "spell_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"

std::unique_ptr<ISpell> SpellHand::create_random_spell() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    
    switch (dis(gen))
    {
    case 0:
        return std::make_unique<DirectDamageSpell>();
    case 1:
        return std::make_unique<AreaDamageSpell>();
    case 2:
        return std::make_unique<TrapSpell>();
    default:
        return std::make_unique<DirectDamageSpell>();
    }
}

SpellHand::SpellHand() 
    : max_size(DEFAULT_MAX_SIZE) {}

SpellHand::SpellHand(const SpellHand& other) : max_size(other.max_size)
{
    for (const auto& spell : other.spells)
    {
        if (spell)
        {
            if (spell->get_name() == "Direct Damage")
            {
                spells.push_back(std::make_unique<DirectDamageSpell>());
            }
            else if (spell->get_name() == "Area Damage")
            {
                spells.push_back(std::make_unique<AreaDamageSpell>());
            }
            else if (spell->get_name() == "Trap")
            {
                spells.push_back(std::make_unique<TrapSpell>());
            }
            for (int i = 0; i < spell->get_cooldown(); ++i)
            {
                spells.back()->update_cooldown();
            }
        }
    }
}

SpellHand::SpellHand(SpellHand&& other) noexcept
    : spells(std::move(other.spells)), max_size(other.max_size)
{
    other.max_size = DEFAULT_MAX_SIZE;
}

SpellHand& SpellHand::operator=(const SpellHand& other)
{
    if (this != &other)
    {
        clear_hand();
        max_size = other.max_size;
        
        for (const auto& spell : other.spells)
        {
            if (spell)
            {
                if (spell->get_name() == "Direct Damage")
                {
                    spells.push_back(std::make_unique<DirectDamageSpell>());
                }
                else if (spell->get_name() == "Area Damage")
                {
                    spells.push_back(std::make_unique<AreaDamageSpell>());
                }
                else if (spell->get_name() == "Trap")
                {
                    spells.push_back(std::make_unique<TrapSpell>());
                }
                for (int i = 0; i < spell->get_cooldown(); ++i)
                {
                    spells.back()->update_cooldown();
                }
            }
        }
    }
    return *this;
}

SpellHand& SpellHand::operator=(SpellHand&& other) noexcept
{
    if (this != &other)
    {
        spells = std::move(other.spells);
        max_size = other.max_size;
        other.max_size = DEFAULT_MAX_SIZE;
    }
    return *this;
}

ISpell* SpellHand::get_spell(size_t index) const
{
    if (index >= spells.size())
    {
        return nullptr;
    }
    return spells[index].get();
}
size_t SpellHand::get_spell_count() const { return spells.size(); }
size_t SpellHand::get_max_size() const { return max_size; }
IDirectDamageSpell* SpellHand::get_direct_damage_spell(size_t index) const
{
    if (index >= spells.size()) return nullptr;
    return dynamic_cast<IDirectDamageSpell*>(spells[index].get());
}
IAreaDamageSpell* SpellHand::get_area_damage_spell(size_t index) const
{
    if (index >= spells.size()) return nullptr;
    return dynamic_cast<IAreaDamageSpell*>(spells[index].get());
}
ITrapSpell* SpellHand::get_trap_spell(size_t index) const
{
    if (index >= spells.size()) return nullptr;
    return dynamic_cast<ITrapSpell*>(spells[index].get());
}

void SpellHand::set_max_size(size_t new_max_size) { max_size = new_max_size; }

bool SpellHand::is_full() const { return spells.size() >= max_size; }
bool SpellHand::is_empty() const { return spells.empty(); }

bool SpellHand::add_spell(std::unique_ptr<ISpell> spell)
{
    if (spells.size() >= max_size || !spell)
    {
        return false;
    }
    
    spells.push_back(std::move(spell));
    return true;
}

void SpellHand::clear_hand()
{
    spells.clear();
}

void SpellHand::remove_half_random()
{
    size_t n = spells.size();
    if (n <= 1) return;

    size_t to_remove = n / 2;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(spells.begin(), spells.end(), gen);
    spells.erase(spells.begin(), spells.begin() + to_remove);
}

void SpellHand::increase_max_size()
{
    max_size += 1;
}

void SpellHand::update_cooldowns()
{
    for (auto& spell : spells)
    {
        if (spell)
        {
            spell->update_cooldown();
        }
    }
}

void SpellHand::initialize_hand()
{
    clear_hand();
    if (!is_full())
    {
        add_spell(create_random_spell());
    }
}

void SpellHand::save_binary(std::ostream& os) const
{
    uint32_t mx = static_cast<uint32_t>(max_size);
    os.write(reinterpret_cast<const char*>(&mx), sizeof(mx));

    uint32_t cnt = static_cast<uint32_t>(spells.size());
    os.write(reinterpret_cast<const char*>(&cnt), sizeof(cnt));

    for (const auto& sptr : spells)
    {
        uint8_t id = 0;
        std::string name = sptr->get_name();
        if (name == "Direct Damage") id = 1;
        else if (name == "Area Damage") id = 2;
        else if (name == "Trap") id = 3;
        else id = 0;

        os.write(reinterpret_cast<const char*>(&id), sizeof(id));
        sptr->save_binary(os);
    }
}

void SpellHand::load_binary(std::istream& is)
{
    uint32_t mx = 0;
    is.read(reinterpret_cast<char*>(&mx), sizeof(mx));
    max_size = static_cast<size_t>(mx);

    uint32_t cnt = 0;
    is.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
    spells.clear();
    spells.reserve(cnt);

    for (uint32_t i = 0; i < cnt; ++i)
    {
        uint8_t id = 0;
        is.read(reinterpret_cast<char*>(&id), sizeof(id));
        std::unique_ptr<ISpell> sp;
        switch (id)
        {
            case 1: sp = std::make_unique<DirectDamageSpell>(); break;
            case 2: sp = std::make_unique<AreaDamageSpell>(); break;
            case 3: sp = std::make_unique<TrapSpell>(); break;
            default: break;
        }
        sp->load_binary(is);
        spells.push_back(std::move(sp));
    }
}
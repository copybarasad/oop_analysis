#ifndef INCLUDE_INVENTORY
#define INCLUDE_INVENTORY

#include <vector>
#include <string>
#include "../items/spell.h"

enum class ItemType {
    Empty,
    HealthPotion,
    Sword,
    Bow,
    Spell
};

class InventoryItem {
private:
    ItemType type_;
    std::string name_;
    int quantity_;
    Spell* spell_;

public:
    InventoryItem();
    InventoryItem(ItemType type, const std::string& name, int quantity = 1);
    InventoryItem(Spell* spell);
    InventoryItem(const InventoryItem& other);
    ~InventoryItem();

    InventoryItem& operator=(const InventoryItem& other);

    ItemType type() const { return type_; }
    std::string name() const { return name_; }
    int quantity() const { return quantity_; }
    const Spell* spell() const { return spell_; }

    void set_quantity(int quantity) { quantity_ = quantity; }
    void add_quantity(int amount) { quantity_ += amount; }
    
    Spell* take_spell();
};

class Inventory {
private:
    int max_slots_;
    std::vector<InventoryItem*> items_;

public:
    Inventory(int max_slots = 5);
    ~Inventory();
    
    Inventory(const Inventory& other);
    Inventory& operator=(const Inventory& other);

    int max_slots() const { return max_slots_; }
    int item_count() const { return items_.size(); }
    const std::vector<InventoryItem*>& items() const { return items_; }

    bool add_item(ItemType type, const std::string& name, int quantity = 1);
    bool add_spell(Spell* spell);
    bool is_full() const;
    InventoryItem* get_item(int slot);
    const InventoryItem* get_item(int slot) const;
    Spell* take_spell_from_slot(int slot);

    void increase_max_slots(int amount);
};

#endif
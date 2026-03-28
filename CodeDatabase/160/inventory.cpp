#include "../include/items/inventory.h"

InventoryItem::InventoryItem() : type_(ItemType::Empty), name_("Пусто"), quantity_(0), spell_(nullptr) {}

InventoryItem::InventoryItem(ItemType type, const std::string& name, int quantity)
    : type_(type), name_(name), quantity_(quantity), spell_(nullptr) {}

InventoryItem::InventoryItem(Spell* spell)
    : type_(ItemType::Spell), name_(spell->name()), quantity_(1), spell_(spell) {}

InventoryItem::InventoryItem(const InventoryItem& other)
    : type_(other.type_), name_(other.name_), quantity_(other.quantity_),
      spell_(other.spell_ ? other.spell_->Clone() : nullptr) {}

InventoryItem::~InventoryItem() {
    delete spell_;
}

InventoryItem& InventoryItem::operator=(const InventoryItem& other) {
    if (this != &other) {
        delete spell_;
        type_ = other.type_;
        name_ = other.name_;
        quantity_ = other.quantity_;
        spell_ = other.spell_ ? other.spell_->Clone() : nullptr;
    }
    return *this;
}

Spell* InventoryItem::take_spell() {
    Spell* temp = spell_;
    spell_ = nullptr;
    return temp;
}

Inventory::Inventory(int max_slots) : max_slots_(max_slots) {
    items_.reserve(max_slots_);
}

Inventory::~Inventory() {
    for (InventoryItem* item : items_) {
        delete item;
    }
}

Inventory::Inventory(const Inventory& other) {
    items_.reserve(max_slots_);
    for (const InventoryItem* item : other.items_) {
        items_.push_back(new InventoryItem(*item));
    }
}

Inventory& Inventory::operator=(const Inventory& other) {
    if (this != &other) {
        for (InventoryItem* item : items_) {
            delete item;
        }
        items_.clear();
        
        for (const InventoryItem* item : other.items_) {
            items_.push_back(new InventoryItem(*item));
        }
    }
    return *this;
}

bool Inventory::add_item(ItemType type, const std::string& name, int quantity) {
    for (size_t i = 0; i < items_.size(); i++) {
        if (items_[i]->type() == type && items_[i]->name() == name && type != ItemType::Spell) {
            items_[i]->add_quantity(quantity);
            return true;
        }
    }

    if (items_.size() >= max_slots_) {
        return false;
    }

    items_.push_back(new InventoryItem(type, name, quantity));
    return true;
}

bool Inventory::add_spell(Spell* spell) {
    if (items_.size() >= max_slots_) {
        return false;
    }
    
    items_.push_back(new InventoryItem(spell));
    return true;
}

bool Inventory::is_full() const {
    return items_.size() >= max_slots_;
}

InventoryItem* Inventory::get_item(int slot) {
    if (slot < 0 || slot >= static_cast<int>(items_.size())) {
        return nullptr;
    }
    return items_[slot];
}

const InventoryItem* Inventory::get_item(int slot) const {
    if (slot < 0 || slot >= static_cast<int>(items_.size())) {
        return nullptr;
    }
    return items_[slot];
}

Spell* Inventory::take_spell_from_slot(int slot) {
    if (slot < 0 || slot >= static_cast<int>(items_.size())) {
        return nullptr;
    }
    
    if (items_[slot]->type() != ItemType::Spell) {
        return nullptr;
    }
    
    Spell* spell = items_[slot]->take_spell();
    delete items_[slot];
    items_.erase(items_.begin() + slot);
    return spell;
}

void Inventory::increase_max_slots(int amount) {
    max_slots_ += amount;
    if (max_slots_ < 1) {
        max_slots_ = 1;
    }
}
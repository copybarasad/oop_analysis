#ifndef OOP_SPELL_HAND_H_
#define OOP_SPELL_HAND_H_

#include "spell.h"
#include <vector>
#include <memory>
#include <optional>

namespace rpg {
    class Player;
    class SpellHand final {
    public:
        static constexpr size_t kDefaultMaxSize = 5;
        
        explicit SpellHand(size_t max_size = kDefaultMaxSize);
        
        SpellHand(const SpellHand& other);
        SpellHand& operator=(const SpellHand& other);
        
        SpellHand(SpellHand&& other) noexcept = default;
        SpellHand& operator=(SpellHand&& other) noexcept = default;
        
        ~SpellHand() = default;
        
        bool AddSpell(std::unique_ptr<Spell> spell);
        
        [[nodiscard]] std::optional<size_t> BuyRandomSpell(Player& player);
        
        [[nodiscard]] const Spell* GetSpell(size_t index) const;
        
        [[nodiscard]] Spell* GetSpellMutable(size_t index);
        
        void RemoveSpell(size_t index);
        
        [[nodiscard]] size_t GetSize() const;
        [[nodiscard]] size_t GetMaxSize() const;
        [[nodiscard]] bool IsFull() const;
        
        void InitializeWithRandomSpell();
        
        [[nodiscard]] bool IsNextCastEnhanced() const;
        void ActivateEnhancement();
        void DeactivateEnhancement();
        
    private:
        std::vector<std::unique_ptr<Spell>> spells_;
        size_t max_size_;
        bool next_cast_enhanced_;
        
        [[nodiscard]] std::unique_ptr<Spell> CreateRandomSpell() const;
    };
}

#endif  // OOP_SPELL_HAND_H_


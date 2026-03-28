#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "spell.h"
#include "enhancement_context.h"
#include <string>

class EnhancementSpell : public Spell {
private:
    int enhancementLevel;
    
public:
    EnhancementSpell();
    
    bool apply(const EnhancementContext& context);
    bool canUse(const EnhancementContext& context) const;
    
    const char* getName() const override;
    std::unique_ptr<Spell> clone() const override;
    std::string getSerializedData() const override;
    
    
    int getEnhancementLevel() const;
    void addEnhancement();
    void resetEnhancement();
};

#endif
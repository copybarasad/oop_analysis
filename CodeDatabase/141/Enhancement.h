#ifndef ENHANCEMENT_H
#define ENHANCEMENT_H

enum class EnhancementType {
    INCREASE_RANGE,
    INCREASE_AREA,
    INCREASE_DAMAGE,
    INCREASE_SUMMON_COUNT
};

struct Enhancement {
    EnhancementType type;
    int value;
};

#endif

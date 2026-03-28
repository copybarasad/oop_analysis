#ifndef ISPELLCONFIG_H
#define ISPELLCONFIG_H

class IBattleCard;
class IUpgradeCard;

class ISpellConfig
{
public:
    virtual ~ISpellConfig() = default;
    virtual void execute(IBattleCard& card) = 0;
    virtual void execute(IUpgradeCard& card) = 0;
};

#endif // ISPELLCONFIG_H

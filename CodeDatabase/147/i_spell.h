#ifndef I_SPELL_H
#define I_SPELL_H

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool isConsumed() const = 0;
    virtual void consume() = 0;
    virtual int damage() const = 0;
    virtual int radius() const = 0;
    virtual std::string name() const = 0;
    virtual std::unique_ptr<ISpell> clone() const = 0;
};

#endif // I_SPELL_H

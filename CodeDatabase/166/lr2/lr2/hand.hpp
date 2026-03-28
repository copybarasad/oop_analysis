#ifndef HAND_HPP_INCLUDED
#define HAND_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include <cstddef>
#include "spell.hpp"

class Hand {
public:
    explicit Hand(std::size_t cap);

    std::size_t capacity() const noexcept { return cap_; }
    std::size_t size() const noexcept;
    bool has(std::size_t i) const noexcept;

    Spell* get(std::size_t i) noexcept;
    const Spell* get(std::size_t i) const noexcept;

    void remove(std::size_t i) noexcept;     // удаляет ровно одну карту по индексу
    std::string cardName(std::size_t i) const;

    bool drawRandom();

private:
    std::size_t cap_;
    std::vector<std::unique_ptr<Spell>> slots_;
};

#endif

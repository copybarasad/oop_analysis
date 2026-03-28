#ifndef HAND_HPP
#define HAND_HPP
#include <vector>
#include <memory>
#include <string>
#include <random>

class ISpell;
class Game;

class PlayerHand {
    std::size_t capacity_;
    std::vector<std::unique_ptr<ISpell>> spells_;
public:
    explicit PlayerHand(std::size_t cap);
    PlayerHand(const PlayerHand&) = delete;
    PlayerHand& operator=(const PlayerHand&) = delete;
    PlayerHand(PlayerHand&&) noexcept = default;
    PlayerHand& operator=(PlayerHand&&) noexcept = default;
    ~PlayerHand();
    std::size_t Capacity() const noexcept;
    std::size_t Size() const noexcept;
    bool Add(std::unique_ptr<ISpell> s);
    bool Use(std::size_t index, Game& game, int tx,int ty);
    const ISpell* Get(std::size_t index) const;
    std::vector<std::string> Names() const;
    std::vector<std::string> Serialize() const;
    void Load(const std::vector<std::string>& codes);
    void DiscardHalf(std::mt19937& rng);
};
#endif

 #ifndef GAME_LOGGING_H
 #define GAME_LOGGING_H

 #include <memory>
 #include <string>

 enum class GameEventType {
 	System,
 	Movement,
 	Damage,
 	Spell,
 	Resource
 };

 struct GameEvent {
 	GameEventType type = GameEventType::System;
 	std::string description;
 };

 class GameLogSink {
 public:
 	virtual ~GameLogSink() = default;
 	virtual void write(const GameEvent& event) = 0;
 };

 class ConsoleLogSink : public GameLogSink {
 public:
 	void write(const GameEvent& event) override;
 };

 class FileLogSink : public GameLogSink {
 public:
 	explicit FileLogSink(const std::string& path);
 	void write(const GameEvent& event) override;

 private:
 	std::string filePath;
 };

 // Простая "шина" событий: сущности уведомляют об игровых событиях,
 // а шина передает их выбранному обработчику.
 class GameEventBus {
 public:
 	explicit GameEventBus(std::shared_ptr<GameLogSink> sink = nullptr);

 	void setSink(std::shared_ptr<GameLogSink> sink);
 	void publish(const GameEvent& event) const;
 	bool hasSink() const { return static_cast<bool>(sink); }

 private:
 	std::shared_ptr<GameLogSink> sink;
 };

 #endif


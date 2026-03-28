 #include "game_logging.h"
 #include <ctime>
 #include <fstream>
 #include <iostream>

 namespace {
 
 std::string timestamp() {
 	std::time_t now = std::time(nullptr);
 	char buffer[32]{};
 	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
 	return buffer;
 }
 
 std::string prefix(GameEventType type) {
 	switch (type) {
 		case GameEventType::Movement: return "[MOVE]";
 		case GameEventType::Damage: return "[DMG ]";
 		case GameEventType::Spell: return "[SPELL]";
 		case GameEventType::Resource: return "[RES ]";
 		case GameEventType::System:
 		default: return "[SYS ]";
 	}
 }
 
 } // namespace
 
 void ConsoleLogSink::write(const GameEvent& event) {
 	std::cout << prefix(event.type) << " " << timestamp() << " " << event.description << "\n";
 }
 
 FileLogSink::FileLogSink(const std::string& path) : filePath(path) {}
 
 void FileLogSink::write(const GameEvent& event) {
 	std::ofstream out(filePath, std::ios::app);
 	if (!out.is_open()) {
 		return;
 	}
 	out << prefix(event.type) << " " << timestamp() << " " << event.description << "\n";
 }
 
 GameEventBus::GameEventBus(std::shared_ptr<GameLogSink> sink)
 	: sink(std::move(sink)) {}
 
 void GameEventBus::setSink(std::shared_ptr<GameLogSink> newSink) {
 	sink = std::move(newSink);
 }
 
 void GameEventBus::publish(const GameEvent& event) const {
 	if (sink) {
 		sink->write(event);
 	}
 }


#include "logger.h"

Logger::Logger(){
    file.open(LOG_FILE, std::ios::app);
    file << "\n=== New Game Session ===\n";
};
    
void Logger::log(const std::string& message){
    file << message << std::endl;
};
    
Logger::~Logger(){
    file << "=== Session End ===\n\n";
    file.close();
};
#ifndef INPUT_FILE_RAII_H
#define INPUT_FILE_RAII_H

#include <fstream>

class InputFileRAII {
private:
	std::ifstream& file;
	
public:
	explicit InputFileRAII(std::ifstream& f);
	~InputFileRAII();
	InputFileRAII(const InputFileRAII&) = delete;
	InputFileRAII& operator=(const InputFileRAII&) = delete;
};

#endif






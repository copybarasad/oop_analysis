#include "input_file_raii.h"

InputFileRAII::InputFileRAII(std::ifstream& f) : file(f) {}

InputFileRAII::~InputFileRAII() {
	if (file.is_open()) {
		file.close();
	}
}






#include "output_file_raii.h"

OutputFileRAII::OutputFileRAII(std::ofstream& f) : file(f) {}

OutputFileRAII::~OutputFileRAII() {
	if (file.is_open()) {
		file.close();
	}
}






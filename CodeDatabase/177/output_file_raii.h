#ifndef OUTPUT_FILE_RAII_H
#define OUTPUT_FILE_RAII_H

#include <fstream>

class OutputFileRAII {
private:
	std::ofstream& file;
	
public:
	explicit OutputFileRAII(std::ofstream& f);
	~OutputFileRAII();
	OutputFileRAII(const OutputFileRAII&) = delete;
	OutputFileRAII& operator=(const OutputFileRAII&) = delete;
};

#endif






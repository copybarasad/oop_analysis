#include "../h/SaveConfiguration.h"

using namespace std;

SaveConfiguration::SaveConfiguration() 
	: saveDirectory("C:/Users/hohra/CLionProjects/untitled4/saves/"), saveExtension(".json"), saveVersion(2) {
}

SaveConfiguration::SaveConfiguration(const string& directory) 
	: saveDirectory(directory), saveExtension(".json"), saveVersion(2) {
}

const string& SaveConfiguration::getSaveDirectory() const {
	return saveDirectory;
}

const string& SaveConfiguration::getSaveExtension() const {
	return saveExtension;
}

int SaveConfiguration::getSaveVersion() const {
	return saveVersion;
}

string SaveConfiguration::buildFilePath(const string& saveName) const {
	return saveDirectory + saveName + saveExtension;
}
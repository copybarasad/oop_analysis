#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include "Protection/TextGifProtector.h"

const bool repair_text_gif = false;
extern bool enable_animations;

class ASCIIAnimationPlayer 
{
private:
    std::vector<std::string> frames_array;
    int fps;
    
    void loadFramesFromFile(const std::string& file_name);
    std::string findAnimationFile(const std::string& animation_name);
    
public:
    ASCIIAnimationPlayer();
    void play(const std::string& animation_name, int repeat_count = 1);
};
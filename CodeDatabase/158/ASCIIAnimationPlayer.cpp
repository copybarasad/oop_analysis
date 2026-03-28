#include "ASCIIAnimationPlayer.h"

bool enable_animations = true;

ASCIIAnimationPlayer::ASCIIAnimationPlayer() 
{
    if (repair_text_gif) TextGifProtector::repairGifFiles(true);
}

void ASCIIAnimationPlayer::loadFramesFromFile(const std::string& file_name) 
{
    if (repair_text_gif) 
    {
        std::string fileContent = TextGifProtector::readProtectedFile(file_name);
        if (fileContent.empty()) 
        {
            std::ifstream input_file(file_name);
            frames_array.clear();
            std::string current_line;
            std::string current_frame_data;
            while (std::getline(input_file, current_line)) 
            {
                if (current_line == "---FRAME_SEPARATOR---") 
                {
                    if (!current_frame_data.empty()) 
                    {
                        frames_array.push_back(current_frame_data);
                        current_frame_data.clear();
                    }
                } 
                else 
                {
                    current_frame_data += current_line + "\n";
                }
            }
            if (!current_frame_data.empty()) frames_array.push_back(current_frame_data);
            input_file.close();
        } 
        else 
        {
            frames_array.clear();
            std::string current_frame_data;
            std::stringstream contentStream(fileContent);
            std::string current_line;
            while (std::getline(contentStream, current_line)) 
            {
                if (current_line == "---FRAME_SEPARATOR---") 
                {
                    if (!current_frame_data.empty()) 
                    {
                        frames_array.push_back(current_frame_data);
                        current_frame_data.clear();
                    }
                } 
                else 
                {
                    current_frame_data += current_line + "\n";
                }
            }
            if (!current_frame_data.empty()) frames_array.push_back(current_frame_data);
        }
    } 
    else 
    {
        std::ifstream input_file(file_name);
        frames_array.clear();
        std::string current_line;
        std::string current_frame_data;
        while (std::getline(input_file, current_line)) 
        {
            if (current_line == "---FRAME_SEPARATOR---") 
            {
                if (!current_frame_data.empty()) 
                {
                    frames_array.push_back(current_frame_data);
                    current_frame_data.clear();
                }
            } 
            else 
            {
                current_frame_data += current_line + "\n";
            }
        }
        if (!current_frame_data.empty()) frames_array.push_back(current_frame_data);
        input_file.close();
    }
}

std::string ASCIIAnimationPlayer::findAnimationFile(const std::string& animation_name) 
{
    DIR* directory;
    struct dirent* directory_entry;
    directory = opendir(".");
    if (directory != NULL) 
    {
        while ((directory_entry = readdir(directory)) != NULL) 
        {
            std::string file_name = directory_entry->d_name;
            if (file_name.find(animation_name) == 0 && file_name.length() > 4) 
            {
                std::string file_extension = file_name.substr(file_name.length() - 4);
                if (file_extension == ".txt") 
                {
                    closedir(directory);
                    return file_name;
                }
            }
        }
        closedir(directory);
    }
    return "";
}

void ASCIIAnimationPlayer::play(const std::string& animation_name, int repeat_count) 
{
    if (animation_name.empty() || !enable_animations) return;
    
    std::string found_file = findAnimationFile(animation_name);
    if (found_file.empty()) return;
    
    size_t underscore_position = found_file.find('_');
    size_t dot_position = found_file.find('.');
    if (underscore_position != std::string::npos && dot_position != std::string::npos) 
    {
        std::string fps_string = found_file.substr(underscore_position + 1, dot_position - underscore_position - 1);
        fps = std::stoi(fps_string);
    } 
    else fps = 15;
    
    loadFramesFromFile(found_file);
    if (frames_array.empty()) return;
    
    int delay_time = 1000 / fps;
    for (int current_loop = 0; current_loop < repeat_count; current_loop++) 
    {
        for (size_t frame_index = 0; frame_index < frames_array.size(); frame_index++) 
        {
            std::cout << "\033c";
            std::cout << frames_array[frame_index];
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
        }
    }
}
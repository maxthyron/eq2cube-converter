
#include "util_functions.h"

// Creates cv::Mat from image file
std::unique_ptr<cv::Mat> getImage(const cv::String &path) {
    std::unique_ptr<cv::Mat> image(new cv::Mat);
    *image = cv::imread(path);
    if ((*image).empty()) {
        std::cout << "Image doesn't exist" << std::endl;
        return nullptr;
    }
    return image;
}

// Returns input and output directories
void getDirectories(std::string &inputDir, std::string &outputDir) {
    inputDir = R"(../input-test/)";
    outputDir = R"(../output-test/)";
}

// Gets all files from dirPath directory
void getFiles(const std::string &dirPath, std::vector<std::string> &files) { // TODO: Refactor - Find better solution
    DIR *dir;
    dirent *pdir;

    dir = opendir(dirPath.c_str());
    if (dir != nullptr) {
        while ((pdir = readdir(dir)) != nullptr) {
            if (strcmp(pdir->d_name, ".") != 0 && strcmp(pdir->d_name, "..") != 0)
                files.emplace_back(pdir->d_name);
        }
        closedir(dir);
    } else {
        std::cout << "Directory not found!" << std::endl;
    }
}

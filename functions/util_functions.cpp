
#include "util_functions.h"

std::unique_ptr<cv::Mat> getImage(const cv::String &path) {
    std::unique_ptr<cv::Mat> image(new cv::Mat);
    *image = cv::imread(path);
    if ((*image).empty()) {
        std::cout << "Image doesn't exist" << std::endl;
        return nullptr;
    }
    return image;
}


void get_directories(std::string &inputDir, std::string &outputDir) {
    inputDir = R"(../test-input/)";
    outputDir = R"(../test-output/)";
}

void get_files(const std::string &dirPath, std::vector<std::string> &files) { // TODO: Refactor - Find better solution
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

#ifndef CONVERTER_UTIL_FUNCTIONS_H
#define CONVERTER_UTIL_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <dirent.h>

std::unique_ptr<cv::Mat> getImage(const cv::String &path);
void get_directories(std::string &inputDir, std::string &outputDir);
void get_files(const std::string &dirPath, std::vector<std::string>& files);

#endif //CONVERTER_UTIL_FUNCTIONS_H

#ifndef CONVERTER_MATH_FUNCTIONS_H
#define CONVERTER_MATH_FUNCTIONS_H

#include <opencv2/opencv.hpp>

#include "./create_maps.h"


#define MAP_SIZE 1000

void createCubeMapFace(std::vector<std::vector<cv::Mat *> *> &maps, const cv::Mat &in, cv::Mat &result);

#endif //CONVERTER_MATH_FUNCTIONS_H

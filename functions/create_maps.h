#ifndef CONVERTER_CREATE_MAPS_H
#define CONVERTER_CREATE_MAPS_H

#include <opencv2/opencv.hpp>

static float faceTransform[6][2] =
        {
                {0,         0},
                {M_PI / 2, 0},
                {M_PI,      0},
                {-M_PI / 2, 0},
                {0,         -M_PI / 2},
                {0,        M_PI / 2}
        };



void createMaps(int inWidth, int inHeight, int faceSize, std::vector<std::vector<cv::Mat*>*> &maps);

#endif //CONVERTER_CREATE_MAPS_H

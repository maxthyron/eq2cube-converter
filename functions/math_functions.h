#ifndef CONVERTER_MATH_FUNCTIONS_H
#define CONVERTER_MATH_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <chrono>

#include "./create_maps.h"

using namespace std::chrono;


inline void createCubeMapFace(const cv::Mat &in, cv::Mat &result) {
    auto clock_start = std::chrono::high_resolution_clock::now();
    int inWidth = in.cols;
    int inHeight = in.rows;

    int faceSize = in.rows;

    std::vector<std::vector<cv::Mat *> *> maps;
    for (int i = 0; i < 6; i++) {
        maps.push_back(new std::vector<cv::Mat *>);
    }
    create_maps(inWidth, inHeight, faceSize, maps);


    cv::Mat face = cv::Mat(faceSize, faceSize, CV_8U, cv::Scalar(0));

    for (int faceId = 0; faceId < 6; faceId++) {
        auto inside_start = std::chrono::high_resolution_clock::now();
        cv::Mat mapx = *(*maps[faceId])[0];
        cv::Mat mapy = *(*maps[faceId])[1];

        int col, row;
        cv::namedWindow("mapx", cv::WINDOW_NORMAL);
        cv::namedWindow("mapy", cv::WINDOW_NORMAL);
        cv::Mat temp1, temp2;
        cv::normalize(mapy, temp1, 0, 1, cv::NORM_MINMAX);
        cv::normalize(mapx, temp2, 0, 1, cv::NORM_MINMAX);
        cv::imshow("mapx", temp2);
        cv::imshow("mapy", temp1);
        cv::waitKey(0);
        cv::destroyAllWindows();

        // Do actual resampling using OpenCV's remap
        cv::remap(in, face, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);

        cv::Mat fixRotate;
        if (faceId < 4) {
            col = (faceId + 1) % 4;
            row = 1;
        } else {
            if (faceId == 4) {
                fixRotate =
                        cv::getRotationMatrix2D(cv::Point(face.rows / 2, face.cols / 2), -90, 1); // Get rid of this fix
            } else {
                fixRotate = cv::getRotationMatrix2D(cv::Point(face.rows / 2 - 1, face.cols / 2), 90, 1); //
            }

            cv::warpAffine(face, face, fixRotate, face.size()); //
            col = 1;
            row = (faceId % 4) * 2;
        }

        face.copyTo(result(cv::Rect(col * faceSize, row * faceSize, faceSize, faceSize)));
        auto inside_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<milliseconds>(inside_stop - inside_start);
        std::cout << "One side: " << duration.count() << " ms" << std::endl;

    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<milliseconds>(stop - clock_start);
    std::cout << "Convert: " << duration.count() << " ms" << std::endl;

    for (int i = 0; i < maps.size(); i++) {
        delete (*maps[i])[0];
        delete (*maps[i])[1];
        delete maps[i];
    }
}

#endif //CONVERTER_MATH_FUNCTIONS_H

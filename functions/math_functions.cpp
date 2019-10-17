#include "math_functions.h"

// Creates cube map by interpolating between two maps and input image
void createCubeMapFace(std::vector<std::vector<cv::Mat *> *> &maps, const cv::Mat &in, cv::Mat &result) {
    int faceSize = in.rows;
    cv::Mat face = cv::Mat(faceSize, faceSize, CV_8U, cv::Scalar(0));
    for (int faceId = 0; faceId < 6; faceId++) {
        cv::Mat mapx = *(*maps[faceId])[0];
        cv::Mat mapy = *(*maps[faceId])[1];

        int col, row;
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
    }
}
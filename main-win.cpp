#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "functions/math_functions.h"
#include "functions/util_functions.h"


int main(int argc, char **argv) {
    std::unique_ptr<cv::Mat> source;
    cv::Mat *result;

    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files;

    getDirectories(inputDir, outputDir);
    getFiles(inputDir, files);

    // TODO: Create function for this
    std::vector<std::vector<cv::Mat *> *> maps; // Creates maps for specific resolution
    for (int i = 0; i < 6; i++) {
        maps.push_back(new std::vector<cv::Mat *>);
    }
    std::unique_ptr<cv::Mat> sample_image = getImage(inputDir + files[0]);
    int inWidth = sample_image->cols;
    int inHeight = sample_image->rows;
    int faceSize = sample_image->rows;
    createMaps(inWidth, inHeight, faceSize, maps);

    for (const std::string &file : files) {
        std::cout << file << "\n";
        source = getImage(inputDir + file);
        if (source != nullptr) {
            result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());

            createCubeMapFace(maps, *source, *result);
            cv::imwrite(outputDir + file, (*result));
        }
    }

    return 0;
}

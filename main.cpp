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


    for (const std::string &file : files) {
        std::cout << inputDir + file << "\n";
        source = getImage(inputDir + file);
        result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());

        createCubeMapFace(*source, *result);
        cv::imwrite(outputDir + "out-" + file, (*result));
    }


    return 0;
}
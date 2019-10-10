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

    get_directories(inputDir, outputDir);
    get_files(inputDir, files);


    if (argc > 1) {
        get_files(argv[1], files);

        for (const std::string &file : files) {
            std::cout << inputDir + file << "\n";
            source = getImage(inputDir + file);
            result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());

            createCubeMapFace(*source, *result);
            cv::imwrite(outputDir + file, (*result));
        }
    } else {
        std::cout << "Directory error!" << std::endl;
    }

    return 0;
}
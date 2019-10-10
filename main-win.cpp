#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "functions/math_functions.h"
#include "functions/util_functions.h"


int main(int argc, char **argv) {
    std::vector<std::string> files;
    std::unique_ptr<cv::Mat> source;
    std::string path = R"(../images/)";
    std::string outputDir = R"(../output/)";
    cv::Mat *result;

    get_files(path, files);

    for (const std::string &file : files) {
        std::cout << path + "\\" + file << "\n";
        source = getImage(path + "\\" + file);
        result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());

        createCubeMapFace(*source, *result);
        cv::imwrite(outputDir + "\\" + file, (*result));
    }

    return 0;
}

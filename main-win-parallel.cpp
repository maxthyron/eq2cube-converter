#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <pthread.h>

#include "functions/math_functions.h"
#include "functions/util_functions.h"


struct thread_data {
    cv::Mat *result;
    cv::Mat *source;
    std::string file;
};

void *thread_function(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    std::string outputDir = R"(C:\Users\User\Desktop\Converter\Converter\x64\Release\output)";
    createCubeMapFace(*(data->source), *(data->result));
    cv::imwrite(outputDir + "\\" + data->file, *(data->result));
    pthread_exit(nullptr);
}

#define NUM_THREADS 5


int main(int argc, char **argv) {
    std::unique_ptr<cv::Mat> source;
    cv::Mat *result;

    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files;

    get_directories(inputDir, outputDir);
    get_files(inputDir, files);

    int rc;

    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];

    int part_length = files.size() / NUM_THREADS;
    std::cout << files.size() << " - " << part_length << std::endl;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int start = part_length * i;
        int end = start + part_length;
        for (int j = start; j < end; ++j) {
            std::cout << inputDir + "\\" + files[j] << "\n";
            source = getImage(inputDir + "\\" + files[j]);
            result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());
            td[i].result = result;
            td[i].source = &(*source);
            td[i].file = files[j];

            rc = pthread_create(&threads[i], nullptr, thread_function, (void *) &td[i]);
        }
    }
    pthread_exit(nullptr);
    return 0;
}

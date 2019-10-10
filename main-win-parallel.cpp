#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <pthread.h>

#include "functions/math_functions.h"
#include "functions/util_functions.h"


struct thread_data {
    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files_part;
};

void *thread_function(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    std::unique_ptr<cv::Mat> source;
    cv::Mat *result;
    for (auto &file : data->files_part) {
        std::cout << data->inputDir + file << "\n";
        source = getImage(data->inputDir + file);
        result = new cv::Mat((*source).rows * 3, (*source).rows * 4, (*source).type());
        createCubeMapFace(*source, *result);
        cv::imwrite(data->outputDir + file, *(result));
    }
    pthread_exit(nullptr);
    return nullptr;
}

#define NUM_THREADS 16


int main(int argc, char **argv) {
    std::clock_t clock_start;
    clock_start = std::clock();

    std::unique_ptr<cv::Mat> source;
    cv::Mat *result;

    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files;

    get_directories(inputDir, outputDir);
    get_files(inputDir, files);

    int rc;

    pthread_t threads[NUM_THREADS]; // Here needs to be only 2/8 threads to split initial data array in two
    struct thread_data td[NUM_THREADS];

    int part_length = (int) files.size() / NUM_THREADS;
    int part_last = (int) files.size() % NUM_THREADS;
    std::cout << files.size() << " - " << part_length << std::endl;
    std::vector<std::string> files_part;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int start = int(i < part_last ? (part_length + 1) * i : files.size() - (NUM_THREADS - i) * part_length);
        int end = start + part_length + (i < part_last);
        files_part = std::vector<std::string>(files.begin() + start, files.begin() + end);
        td[i].inputDir = inputDir;
        td[i].outputDir = outputDir;
        td[i].files_part = files_part;
        rc = pthread_create(&threads[i], nullptr, thread_function, (void *) &td[i]);
    }
    for (auto &thread : threads) {
        pthread_join(thread, nullptr);
    }

    std::cout << "\n" << "TIME: " << (std::clock() - clock_start) / (double) (CLOCKS_PER_SEC / 1000.0f) << "ms"
              << std::endl;

    return 0;
}

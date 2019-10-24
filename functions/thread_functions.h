#ifndef CONVERTER_THREAD_FUNCTIONS_H
#define CONVERTER_THREAD_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <pthread.h>

#include "functions/math_functions.h"
#include "functions/util_functions.h"

#define NUM_THREADS 16


struct thread_data {
    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files_part;
};

void *thread_function(void *threadarg);
void thread_convert();

#endif //CONVERTER_THREAD_FUNCTIONS_H

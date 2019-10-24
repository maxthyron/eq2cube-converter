#include "thread_functions.h"

sem_t mutex;

// Processes chunk of data
void *thread_function(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    std::unique_ptr<cv::Mat> source;
    cv::Mat result;

    for (auto &file : data->files_part) {
        sem_wait(&mutex);
        std::cout << data->inputDir + file << "\n";
        sem_post(&mutex);
        source = getImage(data->inputDir + file); // Get image
        result = cv::Mat(source->rows * 3, source->rows * 4, source->type()); // Create result image
        createCubeMapFace(data->maps, *source, result); // Fill result image
        cv::imwrite(data->outputDir + file, result); // Save result image
    }

    pthread_exit(nullptr);
    return nullptr;
}

void thread_convert() {
    std::unique_ptr<cv::Mat> source;

    std::string inputDir;
    std::string outputDir;
    std::vector<std::string> files;

    getDirectories(inputDir, outputDir); // Get directories' strings from hardcoded function
    getFiles(inputDir, files); // Get files from the directory
    if (files.empty()) {
        std::cout << "No files found" << std::endl;
        return;
    }

    int rc;
    std::vector<pthread_t> threads(NUM_THREADS); // Create thread arrays and data arrays for them
    std::vector<struct thread_data> td(NUM_THREADS);

    int part_length = (int) files.size() / NUM_THREADS; // Divide data in chunks for threads
    int part_last = (int) files.size() % NUM_THREADS;
    std::cout << files.size() << " - " << part_length << std::endl;
    std::vector<std::string> files_part;

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

    auto clock_start = std::chrono::high_resolution_clock::now();
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_THREADS; ++i) {
        int block_start = int(i < part_last ? (part_length + 1) * i : files.size() - (NUM_THREADS - i) * part_length);
        int block_end = block_start + part_length + (i < part_last);
        files_part = std::vector<std::string>(files.begin() + block_start, files.begin() + block_end);

        td[i].inputDir = inputDir; // Fill data structure
        td[i].outputDir = outputDir;
        td[i].files_part = files_part;
        td[i].maps = maps;

        rc = pthread_create(&threads[i], nullptr, thread_function, &td[i]); // Create thread
    }
    for (auto &thread : threads) {
        pthread_join(thread, nullptr);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - clock_start);
    std::cout << "Overall time: " << duration.count() << " ms" << std::endl;

    for (auto & map : maps) { // Free all allocated memory
        delete (*map)[0];
        delete (*map)[1];
        delete map;
    }

    return;
}


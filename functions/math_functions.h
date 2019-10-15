#ifndef CONVERTER_MATH_FUNCTIONS_H
#define CONVERTER_MATH_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std::chrono;

static float faceTransform[6][2] =
        {
                {0,         0},
                {M_PI / 2, 0},
                {M_PI,      0},
                {-M_PI / 2, 0},
                {0,         -M_PI / 2},
                {0,        M_PI / 2}
        };

inline void createCubeMapFace(const cv::Mat &in, cv::Mat &result) {
    auto clock_start = std::chrono::high_resolution_clock::now();
    double inWidth = in.cols;
    double inHeight = in.rows;

    int faceSize = in.rows;

    // Allocate map
    cv::Mat mapx(faceSize, faceSize, CV_32F); // it's a square
    cv::Mat mapy(faceSize, faceSize, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    //to our cube face.
    const auto an = (float) sin(M_PI / 4); // Maybe use class for this
    const auto ak = (float) cos(M_PI / 4);
    float nx;
    float ny;
    float ftu;
    float ftv;
    float u, v;
    float d;
    int col, row;

    cv::Mat face = cv::Mat(faceSize, faceSize, CV_8U, cv::Scalar(0));


    for (int faceId = 0; faceId < 6; faceId++) {
        auto inside_start = std::chrono::high_resolution_clock::now();
        ftu = faceTransform[faceId][0];
        ftv = faceTransform[faceId][1];

        // For each point in the target image,
        // calculate the corresponding source coordinates.
        for (int y = 0; y < faceSize; y++) { // Parallel this
            for (int x = 0; x < faceSize; x++) {

                // Map face pixel coordinates to [-1, 1] on plane
                nx = 2 * ((float) y / (float) faceSize - 0.5f);
                ny = 2 * ((float) x / (float) faceSize - 0.5f);


                // Map [-1, 1] plane coords to [-an, an]
                // thats the coordinates in respect to a unit sphere
                // that contains our box.
                nx *= an;
                ny *= an;

                // Project from plane to sphere surface.
                if (ftv == 0) {
                    // Center faces
                    u = atan2(nx, ak);
                    v = atan2(ny * cos(u), ak);
                    u += ftu;
                } else if (ftv > 0) {
                    // Bottom face
                    d = sqrt(nx * nx + ny * ny);
                    v = M_PI / 2 - atan2(d, ak);
                    u = atan2(ny, nx);
                } else {
                    // Top face
                    d = sqrt(nx * nx + ny * ny);
                    v = (float) -M_PI / 2 + atan2(d, ak);
                    u = atan2(-ny, nx);
                }

                // Map from angular coordinates to [-1, 1], respectively.
                u = u / (float) M_PI;
                v = v / (float) (M_PI / 2);

                // Warp around, if our coordinates are out of bounds.
                while (v < -1) {
                    v += 2;
                    u += 1;
                }
                while (v > 1) {
                    v -= 2;
                    u += 1;
                }

                while (u < -1) {
                    u += 2;
                }
                while (u > 1) {
                    u -= 2;
                }

                // Map from [-1, 1] to in texture space
                u = u / 2.0f + 0.5f;
                v = v / 2.0f + 0.5f;

                u = (float) (u * (inWidth - 1));
                v = (float) (v * (inHeight - 1));

                // Save the result for this pixel in map
                mapx.at<float>(x, y) = u;
                mapy.at<float>(x, y) = v;
            }
        }

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
}

#endif //CONVERTER_MATH_FUNCTIONS_H

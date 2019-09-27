#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <string>

float faceTransform[6][2] =
    {
        {0, 0},
        {M_PI / 2, 0},
        {M_PI, 0},
        {-M_PI / 2, 0},
        {0, -M_PI / 2},
        {0, M_PI / 2}
    };

std::unique_ptr<cv::Mat> getImage(const cv::String &path) {
    std::unique_ptr<cv::Mat> image(new cv::Mat);
    *image = cv::imread(path);
    if ((*image).empty()) {
        std::cout << "Image doesn't exist" << std::endl;
        std::cin.get();
        return nullptr;
    }
    return image;
}

inline void createCubeMapFace(const cv::Mat &in, std::vector<cv::Mat *> faces,
                              int faceId = 0, const int width = -1,
                              const int height = -1) {

    float inWidth = float(in.cols);
    float inHeight = float(in.rows);

    // Allocate map
    cv::Mat mapx(height, width, CV_32F);
    cv::Mat mapy(height, width, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    //to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    const float ftu = faceTransform[faceId][0];
    const float ftv = faceTransform[faceId][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Map face pixel coordinates to [-1, 1] on plane
            float nx = (float) y / (float) height - 0.5f;
            float ny = (float) x / (float) width - 0.5f;

            nx *= 2;
            ny *= 2;

            // Map [-1, 1] plane coords to [-an, an]
            // thats the coordinates in respect to a unit sphere
            // that contains our box.
            nx *= an;
            ny *= an;

            float u, v;

            // Project from plane to sphere surface.
            if (ftv == 0) {
                // Center faces
                u = atan2(nx, ak);
                v = atan2(ny * cos(u), ak);
                u += ftu;
            } else if (ftv > 0) {
                // Bottom face
                float d = sqrt(nx * nx + ny * ny);
                v = M_PI / 2 - atan2(d, ak);
                u = atan2(ny, nx);
            } else {
                // Top face
                float d = sqrt(nx * nx + ny * ny);
                v = -M_PI / 2 + atan2(d, ak);
                u = atan2(-ny, nx);
            }

            // Map from angular coordinates to [-1, 1], respectively.
            u = u / (M_PI);
            v = v / (M_PI / 2);

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

            u = u * (inWidth - 1);
            v = v * (inHeight - 1);

            // Save the result for this pixel in map
            mapx.at<float>(x, y) = u;
            mapy.at<float>(x, y) = v;
        }
    }

    cv::Mat &face = (*faces[faceId]);
    // Recreate output image if it has wrong size or type.
    if (face.cols != width || face.rows != height ||
        face.type() != in.type()) {
        face = cv::Mat(width, height, in.type());
    }

    // Do actual resampling using OpenCV's remap
    cv::remap(in, face, mapx, mapy,
              cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
}

// Mat image(600, 800, CV_8UC3, Scalar(100, 250, 30));
int main() {
    std::unique_ptr<cv::Mat> source = getImage("../image.jpg");
    std::cout << (*source).cols << " : " << (*source).rows << std::endl;
    std::vector<cv::Mat *> images = std::vector<cv::Mat *>();
    for (int i = 0; i < 6; i++) {
        images.push_back(new cv::Mat());
    }
    //    cv::Mat *image = new cv::Mat((*source).rows, (*source).cols, (*source).type());

    for (int i = 0; i < 6; i++)
        createCubeMapFace(*source, images, i, (*source).rows, (*source).rows);

    cv::String windowReference = "Reference";
    cv::String windowResult = "Result";
    for (int i = 0; i < 6; i++)
        cv::namedWindow(windowResult + cv::format("%d", i));
    for (int i = 0; i < 6; i++) {
        cv::imshow(windowResult + cv::format("%d", i), (*images[i]));
        cv::imwrite(windowResult + cv::format("%d.jpg", i), (*images[i]));
    }
    cv::namedWindow(windowReference);
    cv::imshow(windowReference, *source);
    cv::waitKey(0);
    cv::destroyWindow(windowResult);
    cv::destroyWindow(windowReference);

    return 0;
}
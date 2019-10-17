#include "create_maps.h"

void createMaps(int inWidth, int inHeight, int faceSize, std::vector<std::vector<cv::Mat*>*> &maps) {
    const auto an = (float) sin(M_PI / 4); // Maybe use class for this
    const auto ak = (float) cos(M_PI / 4);
    float nx;
    float ny;
    float ftu;
    float ftv;
    float u, v;
    float d;

    for (int faceId = 0; faceId < 6; faceId++) {
        ftu = faceTransform[faceId][0];
        ftv = faceTransform[faceId][1];
        // Allocate map
        std::cout << faceSize << std::endl;
        auto *mapx = new cv::Mat(faceSize, faceSize, CV_32F); // it's a square
        auto *mapy = new cv::Mat(faceSize, faceSize, CV_32F);

        // Calculate adjacent (ak) and opposite (an) of the
        // triangle that is spanned from the sphere center
        //to our cube face.

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
                (*mapx).at<float>(x, y) = u;
                (*mapy).at<float>(x, y) = v;
            }
        }

        (*maps[faceId]).push_back(mapx);
        (*maps[faceId]).push_back(mapy);
    }
}
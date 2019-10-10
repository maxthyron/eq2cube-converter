#ifndef CONVERTER_MATH_FUNCTIONS_H
#define CONVERTER_MATH_FUNCTIONS_H

static double faceTransform[6][2] =
        {
                {0,         0},
                {M_PI / 2, 0},
                {M_PI,      0},
                {-M_PI / 2, 0},
                {0,         -M_PI / 2},
                {0,        M_PI / 2}
        };


inline void createCubeMapFace(const cv::Mat &in, cv::Mat &result) {
    double inWidth = in.cols;
    double inHeight = in.rows;

    int faceSize = in.rows;

    // Allocate map
    cv::Mat mapx(faceSize, faceSize, CV_32F);
    cv::Mat mapy(faceSize, faceSize, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    //to our cube face.
    const double an = sin(M_PI / 4); // Maybe use class for this
    const double ak = cos(M_PI / 4);
    double nx;
    double ny;
    float ftu;
    float ftv;
    double u, v;
    double d;
    int col, row;

    cv::Mat face = cv::Mat(faceSize, faceSize, CV_8U, cv::Scalar(0));

    for (int faceId = 0; faceId < 6; faceId++) {
        ftu = faceTransform[faceId][0];
        ftv = faceTransform[faceId][1];

        // For each point in the target image,
        // calculate the corresponding source coordinates.
        for (int y = 0; y < faceSize; y++) { // Parallel this
            for (int x = 0; x < faceSize; x++) {

                // Map face pixel coordinates to [-1, 1] on plane
                nx = 2 * ((double) y / (double) faceSize - 0.5f);
                ny = 2 * ((double) x / (double) faceSize - 0.5f);


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

        // Do actual resampling using OpenCV's remap
        cv::remap(in, face, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);

        cv::Mat fixRotate;
        if (faceId < 4) {
            col = (faceId + 1) % 4;
            row = 1;
        } else {
            if (faceId == 4) {
                fixRotate = cv::getRotationMatrix2D(cv::Point(face.rows / 2, face.cols / 2), -90,
                                                    1); // Get rid of this fix
            } else {
                fixRotate = cv::getRotationMatrix2D(cv::Point(face.rows / 2 - 1, face.cols / 2), 90, 1); //
            }


            cv::warpAffine(face, face, fixRotate, face.size()); //
            col = 1;
            row = (faceId % 4) * 2;
        }

        face.copyTo(result(cv::Rect(col * faceSize, row * faceSize, faceSize, faceSize)));
    }
}

#endif //CONVERTER_MATH_FUNCTIONS_H

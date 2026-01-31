#ifndef COLOR_PICKER_HPP
#define COLOR_PICKER_HPP

#include <opencv2/opencv.hpp>

class ColorPicker {
public:
    explicit ColorPicker(int cameraIndex = 0);
    bool update();

    cv::Scalar getLower() const;
    cv::Scalar getUpper() const;

private:
    cv::VideoCapture cap;
    cv::Mat frame, hsv, mask;

    int hmin = 0, hmax = 179;
    int smin = 0, smax = 255;
    int vmin = 0, vmax = 255;
};

#endif

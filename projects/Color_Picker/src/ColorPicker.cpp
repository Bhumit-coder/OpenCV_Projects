#include "ColorPicker.hpp"
#include <stdexcept>

using namespace cv;

ColorPicker::ColorPicker(int cameraIndex) {
    cap.open(cameraIndex);
    if (!cap.isOpened())
        throw std::runtime_error("Camera open failed");

    namedWindow("Image", WINDOW_NORMAL);
    namedWindow("Mask", WINDOW_NORMAL);
    namedWindow("Trackbars", WINDOW_NORMAL);

    resizeWindow("Trackbars", 640, 200);

    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);
}

bool ColorPicker::update() {
    cap.read(frame);
    if (frame.empty())
        return false;

    cvtColor(frame, hsv, COLOR_BGR2HSV);

    inRange(hsv,
            Scalar(hmin, smin, vmin),
            Scalar(hmax, smax, vmax),
            mask);

    imshow("Image", frame);
    imshow("Mask", mask);

    int key = waitKey(1);
    if (key == 27)  // ESC
        return false;

    return true;
}

Scalar ColorPicker::getLower() const {
    return Scalar(hmin, smin, vmin);
}

Scalar ColorPicker::getUpper() const {
    return Scalar(hmax, smax, vmax);
}

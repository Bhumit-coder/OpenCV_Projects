#ifndef VIRTUAL_PAINTER_HPP
#define VIRTUAL_PAINTER_HPP

#include <opencv2/opencv.hpp>
#include <vector>

/*
  VirtualPainter
  Tracks colored objects and draws their motion on screen
 */
class VirtualPainter {
public:
    // Initializes painter with calibrated HSV ranges
    VirtualPainter(const std::vector<cv::Scalar>& lower,
                   const std::vector<cv::Scalar>& upper);

    // Starts painter loop
    void run(int cameraIndex = 0);

private:
    // HSV bounds for detection
    std::vector<cv::Scalar> lowerHSV;
    std::vector<cv::Scalar> upperHSV;

    // Drawing color per HSV range
    std::vector<cv::Scalar> drawColors;

    // Last detected point per color
    std::vector<cv::Point> lastPoints;

    // Finds centroid of largest contour
    cv::Point getCenter(const cv::Mat& mask);
};

#endif


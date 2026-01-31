#ifndef HSV_CALIBRATOR_HPP
#define HSV_CALIBRATOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

/*
  HSVCalibrator
  Allows user to interactively select one or more HSV ranges
  using trackbars and live mask preview
 */
class HSVCalibrator {
public:
    // Runs calibration loop
    void run(int cameraIndex = 0);

    // Returns saved HSV lower bounds
    std::vector<cv::Scalar> getLowerHSV() const;

    // Returns saved HSV upper bounds
    std::vector<cv::Scalar> getUpperHSV() const;

private:
    // Trackbar values
    int hmin = 0, smin = 0, vmin = 0;
    int hmax = 179, smax = 255, vmax = 255;

    // Stored HSV ranges (one entry per color)
    std::vector<cv::Scalar> lowerHSV;
    std::vector<cv::Scalar> upperHSV;
};

#endif

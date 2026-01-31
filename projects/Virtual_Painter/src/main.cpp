#include "HSVCalibrator.hpp"
#include "VirtualPainter.hpp"
#include <opencv2/core/utils/logger.hpp>

/*
  Entry point
  1. Run HSV calibration (user-defined colors)
  2. Launch Virtual Painter using captured HSV ranges
 */
int main()
{
    // Silence OpenCV internal logs
    cv::utils::logging::setLogLevel(
        cv::utils::logging::LOG_LEVEL_ERROR
    );

    // Step 1: User selects colors using HSV trackbars
    HSVCalibrator calibrator;
    calibrator.run();

    // Retrieve calibrated HSV ranges
    auto lower = calibrator.getLowerHSV();
    auto upper = calibrator.getUpperHSV();

    // Exit if no color was captured
    if (lower.empty())
        return 0;

    // Step 2: Start virtual painter with selected colors
    VirtualPainter painter(lower, upper);
    painter.run();

    return 0;
}

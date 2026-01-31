#include "HSVCalibrator.hpp"
using namespace cv;
using namespace std;

/*
  Runs interactive HSV calibration.
  User specifies how many colors to detect and tunes HSV ranges
  using trackbars with live mask feedback.
 */
void HSVCalibrator::run(int cameraIndex)
{
    VideoCapture cap(cameraIndex);
    if (!cap.isOpened()) return;

    // Number of colors user wants to detect
    int numColors = 0;
    cout << "Enter number of colors to detect: ";
    cin >> numColors;

    // Calibrate each color independently
    for (int i = 0; i < numColors; ++i) {
        cout << "Calibrating color " << i + 1 << endl;

        // Separate windows for clarity
        string trackbarWin = "HSV Trackbars - Color " + to_string(i + 1);
        string maskWin     = "HSV Mask - Color " + to_string(i + 1);

        namedWindow(trackbarWin, WINDOW_NORMAL);
        namedWindow(maskWin, WINDOW_NORMAL);

        // Trackbars for HSV tuning
        createTrackbar("H Min", trackbarWin, &hmin, 179);
        createTrackbar("H Max", trackbarWin, &hmax, 179);
        createTrackbar("S Min", trackbarWin, &smin, 255);
        createTrackbar("S Max", trackbarWin, &smax, 255);
        createTrackbar("V Min", trackbarWin, &vmin, 255);
        createTrackbar("V Max", trackbarWin, &vmax, 255);

        Mat frame, hsv, mask;

        /*
          Live preview loop.
          Press 'n' to lock current HSV range for this color.
         */
        while (true) {
            cap >> frame;
            if (frame.empty()) break;

            cvtColor(frame, hsv, COLOR_BGR2HSV);

            // Generate binary mask for current HSV values
            inRange(
                hsv,
                Scalar(hmin, smin, vmin),
                Scalar(hmax, smax, vmax),
                mask
            );

            imshow(maskWin, mask);

            char key = static_cast<char>(waitKey(1));
            if (key == 'n') {
                lowerHSV.push_back(Scalar(hmin, smin, vmin));
                upperHSV.push_back(Scalar(hmax, smax, vmax));
                break;
            }
        }

        // Cleanup before next color
        destroyWindow(trackbarWin);
        destroyWindow(maskWin);
    }

    destroyAllWindows();
}

/*
  Returns lower HSV bounds for all calibrated colors
 */
vector<Scalar> HSVCalibrator::getLowerHSV() const
{
    return lowerHSV;
}

/*
  Returns upper HSV bounds for all calibrated colors
 */
vector<Scalar> HSVCalibrator::getUpperHSV() const
{
    return upperHSV;
}

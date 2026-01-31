#include "VirtualPainter.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
  Constructor
  Initializes painter with user-calibrated HSV ranges.
  Each HSV range corresponds to one drawable color.
 */
VirtualPainter::VirtualPainter(const vector<Scalar>& lower,
                               const vector<Scalar>& upper)
{
    lowerHSV = lower;
    upperHSV = upper;

    // One last point per color for stroke continuity
    lastPoints.resize(lowerHSV.size(), Point(-1, -1));

    /*
      Generate drawing colors.
      Each color is derived from the midpoint of its HSV range
      so the stroke visually matches the detected object.
     */
    for (size_t i = 0; i < lowerHSV.size(); ++i) {
        Scalar midHSV(
            (lowerHSV[i][0] + upperHSV[i][0]) * 0.5,
            (lowerHSV[i][1] + upperHSV[i][1]) * 0.5,
            (lowerHSV[i][2] + upperHSV[i][2]) * 0.5
        );

        Mat hsv(1, 1, CV_8UC3, midHSV);
        Mat bgr;
        cvtColor(hsv, bgr, COLOR_HSV2BGR);

        Vec3b c = bgr.at<Vec3b>(0, 0);
        drawColors.push_back(Scalar(c[0], c[1], c[2]));
    }
}

/*
  Returns the pen/brush tip position.
  Uses the top-most point of the largest valid contour
  to avoid centroid lag for elongated objects.
 */
Point VirtualPainter::getCenter(const Mat& mask)
{
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    double maxArea = 0.0;
    Point tip(-1, -1);

    for (const auto& c : contours) {
        double area = contourArea(c);

        // Ignore small noisy contours
        if (area > maxArea && area > 500) {
            maxArea = area;

            // Find top-most point (pen tip)
            Point top = c[0];
            for (const auto& p : c) {
                if (p.y < top.y)
                    top = p;
            }
            tip = top;
        }
    }

    return tip;
}

/*
  Main painter loop.
  Tracks each calibrated color and draws strokes
  following pen/brush motion and direction.
 */
void VirtualPainter::run(int cameraIndex)
{
    VideoCapture cap(cameraIndex);
    if (!cap.isOpened())
        return;

    Mat frame, hsv, mask;
    Mat canvas;

    namedWindow("Virtual Painter", WINDOW_NORMAL);

    while (true) {
        cap >> frame;
        if (frame.empty())
            break;

        // Initialize canvas once
        if (canvas.empty())
            canvas = Mat::zeros(frame.size(), CV_8UC3);

        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // Process each calibrated color independently
        for (size_t i = 0; i < lowerHSV.size(); ++i) {
            inRange(hsv, lowerHSV[i], upperHSV[i], mask);

            Point current = getCenter(mask);

            /*
              Draw continuous stroke only when both
              current and previous positions are valid.
             */
            if (current.x != -1 && lastPoints[i].x != -1) {
                line(canvas,
                     lastPoints[i],
                     current,
                     drawColors[i],
                     5,
                     LINE_AA);
            }

            lastPoints[i] = current;
        }

        // Overlay drawing on live feed
        Mat output;
        add(frame, canvas, output);
        imshow("Virtual Painter", output);

        // Exit on ESC
        if (waitKey(1) == 27)
            break;
    }

    destroyAllWindows();
}

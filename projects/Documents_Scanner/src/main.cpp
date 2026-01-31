#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/utils/logger.hpp>

using namespace std;
using namespace cv;


/////////////////////////////// Document-Scanner ///////////////////////////////

Mat img, imgGray, imgBlur, imgCanny, imgDil, imgCrop;
float w = 400.0f, h = 500.0f;
Mat matrix, imgWrap;

/*
  Pre-processing:
  Gray → Blur → Canny → Dilation
  Strengthens edges for stable contour detection
 */
Mat imgDilate(const Mat& img) {
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgCanny, imgDil, kernel);

    return imgDil;
}

void findCoordinates(const Mat& imgDil) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        // cout << area << endl;  (USED TO DIFFERENTIATE DOCUMENT FROM OTHER RECTANGLES)

        vector<vector<Point>> conPoly(contours.size());

        double peri = arcLength(contours[i], true);
        approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

        int no_of_sides = static_cast<int>(conPoly[i].size());

        if (no_of_sides == 4 && area > 1000) {
            for (int j = 0; j < no_of_sides; j++) {
                float x = static_cast<float>(conPoly[i][j].x);
                float y = static_cast<float>(conPoly[i][j].y);

                cout << (j + 1) << "th x coordinate is " << x << endl; // (USED TO FIND ABSCISSA OF DOCUMENT EDGES)
                cout << (j + 1) << "th y coordinate is " << y << endl; // (USED TO FIND ORDINATES OF DOCUMENT EDGES)

                /*
                putText(img, "Hi", Point(x,y), FONT_HERSHEY_DUPLEX, j+1,
                        Scalar(255, 0, 255), 2);
                (IT IS USED TO FIND THE SEQUENCE OF POINT COORDINATES)
                */
            }
        }
    }
}

Mat imgWrap_Crop() {
    Point2f src[4] = {
        {188.f, 221.f},
        {42.f,  564.f},
        {461.f, 637.f},
        {448.f, 258.f}
    };

    Point2f dst[4] = {
        {0.f, 0.f},
        {0.f, h},
        {w,   h},
        {w,   0.f}
    };

    matrix = getPerspectiveTransform(src, dst);

    warpPerspective(
        img,
        imgWrap,
        matrix,
        Size(static_cast<int>(w), static_cast<int>(h))
    );

    imshow("Image Wrap", imgWrap);

    int cropValue = 7;  // (CHANGE VALUE ACCORDING TO AMOUNT OF BACKGROUND VISIBLE IN WRAPPED IMAGE)

    Rect roi(
        cropValue,
        cropValue,
        static_cast<int>(w) - 2 * cropValue,
        static_cast<int>(h) - 2 * cropValue
    );

    imgCrop = imgWrap(roi);
    return imgCrop;
}

/*
    Document Scanner – Entry Point

    - Loads input image from project Resources
    - Preprocesses image to detect document contours
    - Applies perspective warp to obtain scanned output
*/

int main(int argc, char** argv)
{
    // Reduce OpenCV internal noise during execution
    cv::utils::logging::setLogLevel(
        cv::utils::logging::LOG_LEVEL_ERROR
    );

    // Default resource path (relative to build/Debug)
    std::string path = "../../Resources/paper.jpg";

    // Optional override via command line
    if (argc > 1) {
        path = argv[1];
    }

    Mat imgOriginal = imread(path);
    if (imgOriginal.empty()) {
        cout << "Error: Image not found at path -> " << path << endl;
        return -1;
    }

    // Resize for consistent processing scale
    resize(imgOriginal, img, Size(), 0.5, 0.5);

    Mat imgDilated = imgDilate(img);

    findCoordinates(imgDilated);

    Mat imgScanned = imgWrap_Crop();

    imshow("Original Image", img);
    imshow("Scanned Image", imgScanned);

    waitKey(0);
    return 0;
}

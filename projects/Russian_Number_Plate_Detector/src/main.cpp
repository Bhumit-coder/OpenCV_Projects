#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/utils/logger.hpp>

#include <iostream>
#include <filesystem>

// Russian Number Plate Detection using Haar Cascade (Webcam)

using namespace std;
using namespace cv;
namespace fs = std::filesystem;



int main(int argc, char** argv)
{
    // Reduce OpenCV internal noise during execution
    cv::utils::logging::setLogLevel(
        cv::utils::logging::LOG_LEVEL_ERROR
    ); 
   // Resolve executable directory (robust against working-directory issues)
    fs::path exePath = fs::absolute(argv[0]).parent_path();

    // Build absolute path to cascade file
    fs::path cascadePath =
        exePath / ".." / ".." / "Resources" / "haarcascade_russian_plate_number.xml";

    CascadeClassifier plateCascade;
    if (!plateCascade.load(cascadePath.string()))
    {
        cerr << "Error: Could not load cascade file.\n"
             << "Expected at: " << cascadePath << endl;
        return -1;
    }

    // Open default webcam
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cerr << "Error: Could not open webcam." << endl;
        return -1;
    }

    Mat frame, gray;
    vector<Rect> plates;

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        // Haar cascades require grayscale input
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        plateCascade.detectMultiScale(
            gray,
            plates,
            1.1,          // scale factor
            5,            // min neighbors
            0,
            Size(60, 20)  // minimum plate size
        );

        for (const Rect& plate : plates)
        {
            rectangle(frame, plate, Scalar(0, 255, 0), 2);
        }

        imshow("Russian Number Plate Detector", frame);

        // ESC to exit
        if (waitKey(1) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}

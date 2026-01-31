#include "ColorPicker.hpp"
#include <opencv2/core/utils/logger.hpp>

int main() {
    cv::utils::logging::setLogLevel(
        cv::utils::logging::LOG_LEVEL_ERROR
    );

    try {
        ColorPicker picker(0);

        while (true) {
            if (!picker.update())
                break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    cv::destroyAllWindows();
    return 0;
}

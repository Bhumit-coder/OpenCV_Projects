**OpenCV Projects (C++ | CMake)**

A curated collection of foundational computer vision projects built using OpenCV (C++) and CMake, focused on real-time processing, classical vision pipelines, and correct project structuring.

These projects are intentionally designed to:

* Emphasize core OpenCV concepts

* Avoid black-box abstractions

* Be buildable, debuggable, and extensible

* Help beginners understand why things work, not just that they work

📁 **Project Overview**
OpenCV_Projects/
│
├── projects/
│   ├── Color_Picker/
│   ├── Virtual_Painter/
│   ├── Documents_Scanner/
│   └── Russian_Number_Plate_Detector/
│
└── README.md


Each project is self-contained and follows the same structure:

<Project_Name>/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── Resources/        (only when required)

**🔧 Build Requirements**

* C++17 or later

* OpenCV ≥ 4.x

* CMake ≥ 3.16

* Windows / Linux (tested on Windows with MSVC)

Make sure OpenCV binaries are available in your system PATH (Windows users).

**🟦 1. Color Picker**
**Purpose**

Interactively identify HSV color ranges from a live camera feed using trackbars.

**Key Concepts**

* BGR → HSV color space conversion

* Real-time trackbars

* Binary masking using inRange

* Practical color calibration workflow

**How It Works**

* Camera feed is converted to HSV

* Trackbars control H, S, and V min/max values

* Masked output updates live

* Final HSV ranges are used by other projects (e.g. Virtual Painter)

Why This Project Matters

Color calibration is a prerequisite for many vision tasks. This project teaches how to do it correctly instead of guessing values.

**🖌️ 2. Virtual Painter**
**Purpose**

Draw on a virtual canvas using a colored pen/marker tracked in real time.

**Key Concepts**

* HSV-based object tracking

* Contour detection

* Centroid computation using moments

* Persistent drawing using an overlay canvas

**How It Works**

1. HSV ranges (from Color Picker) isolate the pen color

2. Largest valid contour is selected

3. Contour center is tracked frame-to-frame

4. Lines are drawn on a separate canvas and blended with the frame

**Design Notes**

* One canvas per session (no flickering)

* Noise filtered via contour area threshold

* Explicit handling of lost tracking frames

**Limitations (Intentionally Left Visible)**

* HSV sensitivity to lighting

* No Kalman filtering (kept simple on purpose)

**📄 3. Document Scanner**
**Purpose**

Simulate a mobile document scanning pipeline using classical image processing.

**Key Concepts**

* Edge detection (Canny)

* Morphological operations

* Contour approximation

* Perspective transformation

* ROI cropping

**Pipeline**

1. Convert image to grayscale

2. Blur and detect edges

3. Dilate edges to strengthen contours

4. Detect largest quadrilateral contour

5. Apply perspective warp

6. Crop borders for clean output

**Why This Is Important**

This project demonstrates how geometry + image processing can solve real-world problems without deep learning.

**Practical Notes**

* Resource paths are resolved at runtime

* Designed to avoid hard-coded absolute paths

* Easy to adapt for camera input later

**🚘 4. Russian Number Plate Detector
Purpose**

Detect Russian vehicle number plates in real time using a webcam.

**Key Concepts**

* Haar Cascade classifiers

* XML-based object detection

* Real-time video processing

* Robust resource loading

**How It Works**

* Webcam (VideoCapture(0)) provides frames

* Haar cascade XML is loaded at runtime

* Plates are detected using detectMultiScale

* Bounding boxes are drawn live

**Important Design Choice**

This project uses classical Haar cascades deliberately:

* Lightweight

* No GPU required

* Easy to understand

* Suitable for embedded or low-resource systems

**Known Limitations**

* Cascade performance depends on training data

* Sensitive to extreme angles and lighting

Detection only (no OCR yet)

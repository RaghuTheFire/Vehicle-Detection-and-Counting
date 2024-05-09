# Vehicle-Detection-and-Counting

This C++ code is designed to detect and count vehicles in a video stream using OpenCV (Open Source Computer Vision Library). 
Here's a breakdown of what the code does: 
1. The code includes the necessary OpenCV header files and the `vector` library from the C++ Standard Template Library (STL).
2. It defines some constants for minimum contour size, offset for the virtual line, and the height of the virtual line.
3. It declares a vector `matches` to store the centroids of detected contours and an integer `vehicles` to keep track of the number of vehicles detected.
4. The `get_centroid` function calculates the centroid (center point) of a given bounding rectangle.
5. In the `main` function, the code opens a video file named "Video.mp4" and sets the frame width and height to 1920x1080 pixels.
6. It reads two consecutive frames from the video, `frame1` and `frame2`.
7. The code enters a loop where it performs the following operations on each pair of consecutive frames:
- Calculates the absolute difference between `frame1` and `frame2` to detect motion.
- Converts the difference image to grayscale.
- Applies Gaussian blur to reduce noise.
- Applies thresholding to create a binary image.
- Performs dilation and closing operations to fill in gaps and remove noise.
- Finds contours in the processed image.
- Draws bounding rectangles around contours that meet the minimum size criteria.
- Draws a virtual line at a specified height.
- Calculates the centroid of each valid contour and stores it in the `matches` vector.
- Checks if any centroid crosses the virtual line, and if so, increments the `vehicles` counter and removes the centroid from the `matches` vector.
- Displays the processed frames and the vehicle count on the screen.
8. The loop continues until the user presses the Esc key (key code 27).
9. Finally, the code cleans up by closing all windows and releasing the video capture object. In summary, this code performs background subtraction, contour detection, and object tracking to detect and count vehicles crossing a virtual line in a video stream. It displays various intermediate steps and the final result with bounding boxes and vehicle count.

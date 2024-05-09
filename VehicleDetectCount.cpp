
#include <opencv2/opencv.hpp>
#include <vector>

const int min_contour_width = 40;
const int min_contour_height = 40;
const int offset = 10;
const int line_height = 550;
std::vector<cv::Point> matches;
int vehicles = 0;

cv::Point get_centroid(int x, int y, int w, int h) {
    int x1 = w / 2;
    int y1 = h / 2;
    int cx = x + x1;
    int cy = y + y1;
    return cv::Point(cx, cy);
}

int main() {
    cv::VideoCapture cap("Video.mp4");
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    if (!cap.isOpened()) {
        return -1;
    }

    cv::Mat frame1, frame2;
    cap.read(frame1);
    cap.read(frame2);

    const double resize_factor = 0.5;

    while (true) {
        cv::Mat d;
        cv::absdiff(frame1, frame2, d);
        cv::imshow("Absolute Difference", cv::resize(d, cv::Size(), resize_factor, resize_factor));

        cv::Mat grey;
        cv::cvtColor(d, grey, cv::COLOR_BGR2GRAY);
        cv::imshow("Grayscale", cv::resize(grey, cv::Size(), resize_factor, resize_factor));

        cv::Mat blur;
        cv::GaussianBlur(grey, blur, cv::Size(5, 5), 0);
        cv::imshow("Gaussian Blur", cv::resize(blur, cv::Size(), resize_factor, resize_factor));

        cv::Mat th;
        cv::threshold(blur, th, 20, 255, cv::THRESH_BINARY);
        cv::imshow("Threshold", cv::resize(th, cv::Size(), resize_factor, resize_factor));

        cv::Mat dilated;
        cv::dilate(th, dilated, cv::Mat::ones(3, 3, CV_8U));
        cv::imshow("Dilated", cv::resize(dilated, cv::Size(), resize_factor, resize_factor));

        cv::Mat closing;
        cv::morphologyEx(dilated, closing, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)));
        cv::imshow("Closing", cv::resize(closing, cv::Size(), resize_factor, resize_factor));

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(closing, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat frame_contours = frame1.clone();
        cv::drawContours(frame_contours, contours, -1, cv::Scalar(0, 255, 0), 2);
        cv::imshow("Contours", cv::resize(frame_contours, cv::Size(), resize_factor, resize_factor));

        for (int i = 0; i < contours.size(); i++) {
            cv::Rect rect = cv::boundingRect(contours[i]);
            bool contour_valid = (rect.width >= min_contour_width) && (rect.height >= min_contour_height);

            if (!contour_valid) {
                continue;
            }

            cv::rectangle(frame1, cv::Point(rect.x - 10, rect.y - 10), cv::Point(rect.x + rect.width + 10, rect.y + rect.height + 10), cv::Scalar(255, 0, 0), 2);
            cv::line(frame1, cv::Point(0, line_height), cv::Point(1200, line_height), cv::Scalar(0, 255, 0), 2);
            cv::Point centroid = get_centroid(rect.x, rect.y, rect.width, rect.height);
            matches.push_back(centroid);
            cv::circle(frame1, centroid, 5, cv::Scalar(0, 255, 0), -1);
            cv::imshow("Bounding Boxes", cv::resize(frame1, cv::Size(), resize_factor, resize_factor));

            for (const cv::Point& p : matches) {
                if (p.y < (line_height + offset) && p.y > (line_height - offset)) {
                    vehicles++;
                    matches.erase(std::remove(matches.begin(), matches.end(), p), matches.end());
                }
            }
        }

        std::string text = "Total Vehicle Detected: " + std::to_string(vehicles);
        cv::putText(frame1, text, cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 170, 0), 2);

        cv::imshow("Vehicle Detection", cv::resize(frame1, cv::Size(), resize_factor, resize_factor));

        int key = cv::waitKey(1);
        if (key == 27) {
            break;
        }

        frame1 = frame2.clone();
        cap.read(frame2);
    }

    cv::destroyAllWindows();
    cap.release();

    return 0;
}


#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "detection.h"

using std::string;
using std::vector;
using std::ostream;
using cv::dnn::Net;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using AsyncLogger = std::shared_ptr<spdlog::logger>;

class FaceDetector {
	cv::CascadeClassifier classifier;
	AsyncLogger logger;
	double scale = 4;
public:
	FaceDetector();
	cv::Rect generate_bounding_box(const cv::Rect& face_box, const cv::Mat&, int w, int h);
	Detection detectAndDisplay(const cv::Mat &);
	void visualize(cv::Mat*, const Detection&);
	vector<float> predict(const cv::Mat& img);
};

#endif
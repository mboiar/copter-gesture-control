#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>

#include <opencv2\opencv.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "face_detection.h"
#include "gesture_detection.h"
#include "tello.h"

using std::string;
using std::vector;
using std::chrono::milliseconds;
using cv::Mat;
using std::atomic;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using AsyncLogger = std::shared_ptr<spdlog::logger>;


class Buffer {
	// TODO thread-safe buffer
	vector<int> _buffer;
	int max_len;
	size_t size, default_val;
public:
	unsigned int buffer_len;
	Buffer(unsigned int bl, size_t size, size_t defalut_val = 0) : max_len(bl), size(size), default_val(default_val) {
		_buffer = vector<int>(size);
	}
	void add(size_t elem);
	size_t get();
};

class Controller {
	constexpr static int buffer_len = 5;
	constexpr static int dw[3] = { 10, 10, 10 };
	constexpr static long WAIT_RC_CONTROL = 500;
	constexpr static long WAIT_BATTERY = 4000;
	constexpr static milliseconds FACE_TIMEOUT = milliseconds(1000);
	constexpr static milliseconds GESTURE_TIMEOUT = milliseconds(1000);
	Tello *tello;
	bool debug;
	Buffer buffer;
	FaceDetector face_detector;
	GestureDetector gesture_detector;
	atomic<int> battery_stat = -1;
	TimePoint _last_gesture = TimePoint();
	TimePoint _last_face = TimePoint();
	bool stop_tello = false;
	bool is_landing = false;
	vector<int> vel = { 0 };
	static const string cv_window_name;
	AsyncLogger logger;
	void _put_battery_on_frame(Mat *);
public:
	Controller(Tello* tello, bool debug) :
		tello(tello),
		debug(debug),
		face_detector(),
		gesture_detector(),
		buffer(buffer_len, GestureCount) {
		string name("CONTROLLER");
		logger = spdlog::get(name);
		if (!logger) {
			logger = spdlog::stdout_color_mt(name);
		}
		logger->set_level(spdlog::level::info);
	};

	void run();
	void detection_step(Mat*);
	void send_command();
	void get_battery_stat();
	void stop();
	//cv::Rect get_gesture_bounds(const cv::Rect&, const cv::Mat&, int w, int h);
};

#endif
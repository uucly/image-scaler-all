#pragma once

#include <opencv2/opencv.hpp>

namespace imagescaler {

	class ImageScalerService {

	public:
		~ImageScalerService();
		void resize(const cv::Mat&, cv::Mat&, int, int);

		std::unique_ptr<cv::Mat> resize(const std::string filePath, int width, int height);

	};
}
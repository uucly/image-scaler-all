#include "imagescalerservice.h"

imagescaler::ImageScalerService::~ImageScalerService() {
	std::cout << "Wurde gekillt" << std::endl;
}

void imagescaler::ImageScalerService::resize(const cv::Mat &inimage,cv::Mat &outimage, int width, int height) {
	std::cout << "normal resize";
	std::cout << &outimage << std::endl;
	cv::resize(inimage, outimage, cv::Size(width, height));
}

std::unique_ptr<cv::Mat> imagescaler::ImageScalerService::resize(const std::string filePath, int width, int height) {
	auto outImage = std::unique_ptr<cv::Mat>(new cv::Mat());
	auto image_path = cv::samples::findFile(filePath);
	auto img = cv::imread(image_path, cv::IMREAD_COLOR);
	if (img.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
		throw std::runtime_error("Could not read image");
	}

	cv::resize(img, *outImage, cv::Size(width, height));
	return outImage;
}
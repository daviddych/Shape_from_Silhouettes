#pragma once
#include <string>
#include <opencv2/opencv.hpp>

class CLoadImage
{
	using string = std::string;
	using Mat = cv::Mat;

public:
	CLoadImage();
	~CLoadImage();

	static Mat Load(const string& file)
	{
		cv::Mat img = cv::imread(file, cv::IMREAD_GRAYSCALE);
		return img;
	}
};
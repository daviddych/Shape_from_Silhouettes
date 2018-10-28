#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

class CLoadProjectMatrix
{
	using string = std::string;
	using Mat = cv::Mat;

public:
	CLoadProjectMatrix();
	~CLoadProjectMatrix();

	static Mat Load(const string& file)
	{
		std::ifstream infile(file);
		if (!infile.is_open())
		{
			return Mat();
		}

		Mat img = Mat::ones(3, 4, CV_32FC1);
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				if(!infile.eof())
					infile >> img.at<float>(r, c);
			}
		}
		return img;
	}
};


#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

class CLoadData
{
	using string = std::string;
	using ImgProj_vec = std::vector<std::pair<cv::Mat, cv::Mat>>;

public:
	// 从影像和对应的外参文件读取数据
	CLoadData();
	~CLoadData();

	// 分别从图像和对应的外参文件中读取图像和P外参矩阵
	bool Load(const string& imgfile, const string& projfile);


	// 存储影像和对应的外参文件
	ImgProj_vec m_proj_img;
};


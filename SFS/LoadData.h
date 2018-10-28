#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

class CLoadData
{
	using string = std::string;
	using ImgProj_vec = std::vector<std::pair<cv::Mat, cv::Mat>>;

public:
	// ��Ӱ��Ͷ�Ӧ������ļ���ȡ����
	CLoadData();
	~CLoadData();

	// �ֱ��ͼ��Ͷ�Ӧ������ļ��ж�ȡͼ���P��ξ���
	bool Load(const string& imgfile, const string& projfile);


	// �洢Ӱ��Ͷ�Ӧ������ļ�
	ImgProj_vec m_proj_img;
};


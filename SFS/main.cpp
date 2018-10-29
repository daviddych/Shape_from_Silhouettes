/////////////////////////////////////////////////////////////////////
// 
// @author: daiyucheng
// @e-mail: 287382545@qq.com
// @Function: Generating 3D point clouds from their 
//			  silhouettes in a sequence of images.
// @Matlab code: please relate to (Thanks):
//               https://github.com/KKeishiro/Shape-from-Silhouettes
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "LoadData.h"
#include "LoadProjectMatrix.h"
#include "ShapefromSilhouettes.h"
#include <algorithm>
#include "DataViewer.h"
#include <chrono>

#pragma  warning(disable:4996)

using string = std::string;
using Mat = cv::Mat;

int main(int argc, char** argv)
{
	CShapefromSilhouettes sfs(18);

	CLoadData dataReader;
	std::string prefix = R"(..\data\david_)";
	const int max_filelen = 256;
	char pa_filename[max_filelen] = { 0 }, img_filename[max_filelen] = { 0 };
	for (int i=0; i<18; ++i)
	{
		sprintf(pa_filename, "%s%02d.pa", prefix.c_str(), i);
		sprintf(img_filename, "%s%02d.jpg", prefix.c_str(), i);
		
		//std::cout << img_filename << std::endl;
		//std::cout << pa_filename << std::endl;

		if(!dataReader.Load(img_filename, pa_filename))
			break;

		memset(pa_filename, 0, max_filelen);
		memset(img_filename, 0, max_filelen);
	}

	std::vector<std::pair<cv::Mat, cv::Mat>>& data = dataReader.m_proj_img;
	cv::String winname1("org_img");
	cv::String winname2("chg_img");
	std::vector<Mat> projs;
	std::vector<Mat> imgs;
	for (auto v : data)
	{
		//std::cout << v.second << std::endl<< std::endl;
		//std::cout << "ch: " << v.first.channels() << std::endl;
		cv::Mat dst;
		cv::threshold(v.first, dst, 100, 255, cv::THRESH_BINARY);
		cv::imshow(winname1, v.first);
		cv::imshow(winname2, dst);
		cv::waitKey(5);
		projs.push_back(v.second); 
		imgs.push_back(v.first);
	}

	// Reconstruct 3D point clouds using SfS algorithm.
//	sfs.boundingBox(0.4, 2.2, -0.3, 1.3, -1.8, 2.7, 64, 64, 128);
	sfs.boundingBox(0.4, 2.2, -0.3, 1.3, -1.8, 2.7, 20, 20, 30);
	sfs.extract_Silhouette(imgs);
	sfs.projMatrix(projs);
	std::chrono::steady_clock::time_point  now = std::chrono::steady_clock::now();
	std::vector<cv::Point3f>& cloudpoints= sfs.visualHull();
	auto end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - now);
	printf("number of points: %d,  cost time: %fs\n", cloudpoints.size(), time_span);

	// Show the reconstructed 3D point clouds.
	pclViewer::CDataViewer viewer;
	viewer.reSet(argc, argv);
	viewer.imShow(cloudpoints);
	
	return 0;
}
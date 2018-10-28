#pragma once
#include <opencv2/opencv.hpp>

class CShapefromSilhouettes
{
#pragma warning(disable:4010)
	using Mat = cv::Mat;
	using MatArray = std::vector<Mat>;
	using IntArray = std::vector<int>;
	using PointCloud = std::vector<cv::Point3f>;
	using FloatArray = std::vector<float>;

public:
	CShapefromSilhouettes(int numCameras=3);
	~CShapefromSilhouettes();

	// 1. Silhouette extraction: from the provided images, using a simple    \
	      thresholding technique.
	void setSilhouetteThresh(float thresh) { m_thresh = thresh; }
	void extract_Silhouette(MatArray& src, float thresh=-1.0);
	void projMatrix(MatArray& prok);
	
	// 2. Define the volume of interest. At first use a larger bounding box  \
	      than necessary, just to make sure the statue has been included.    \
          Then make the bounding box tight to get better resolution.
	void boundingBox(float x_min, float x_max, float y_min, float y_max,
		float z_min, float z_max, ushort nx, ushort ny, ushort nz);

	// 3. Visual hull: Compute the occupancy score at each voxel. For each   \
	      voxel, transform the point from volume to world coordinates using  \
          the transformation provided.Then project the points into the image.\
          If a projected point falls within the silhouette, add 1 to the score.
	PointCloud& visualHull();

	
private:
	// 图像阈值处理
	void extract_Silhouette(Mat& src, Mat& dst);
	// 生成minv,maxv之间的n点行线性向量数组
	void linspace(float minv, float maxv, ushort n, FloatArray& v);

	// Define a bounding box tight the object.
	float m_box_x[2]{ 0.4f, 2.2f }, m_box_y[2]{ -0.3f, 1.3f }, m_box_z[2]{ -1.8f, 2.8f };
	ushort m_nx{ 20 }, m_ny{ 20 }, m_nz{ 30 };

	// 用在阈值处理
	float m_thresh{ 100 };

	// 图像外参
	MatArray m_projs;

	// 阈值处理后的mat
	MatArray   m_sils;

	// Define transformation from volume to world coordinates.
	Mat m_T;

	// 空间切分体元
	MatArray m_voxels;
	IntArray m_volume;
	ushort   m_volumeThreshold{2};

	// 最终生成的点云
	PointCloud m_PointCloud; 
};
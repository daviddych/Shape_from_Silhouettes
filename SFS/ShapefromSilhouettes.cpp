#include "ShapefromSilhouettes.h"



CShapefromSilhouettes::CShapefromSilhouettes(int numCameras)
{
	m_sils.resize(numCameras);
	m_volumeThreshold = numCameras - 1;
}


CShapefromSilhouettes::~CShapefromSilhouettes()
{
}

// 1. Silhouette extraction:
void CShapefromSilhouettes::extract_Silhouette(MatArray& src, float thresh)
{
	if (m_sils.size() != src.size())
		m_sils.resize(src.size());

	if(thresh > 0.0)
		m_thresh = thresh;

	for (int i = 0; i < src.size(); ++i)
	{
		if (src[i].channels() == 3)
			cv::cvtColor(src[i], src[i], CV_BGR2GRAY);

		extract_Silhouette(src[i], m_sils[i]);
	}
}

void CShapefromSilhouettes::extract_Silhouette(Mat& src, Mat& dst)
{
	cv::threshold(src, dst, m_thresh, 255, cv::THRESH_BINARY);
}

void CShapefromSilhouettes::projMatrix(MatArray& projs)
{
	Mat A = Mat::eye(3, 3, CV_32FC1);
	A.at<float>(0, 2) = 1;
	A.at<float>(1, 2) = 1;
	
	for (auto& p : projs)
	{
		p = A * p;
	}
	m_projs.resize(projs.size());
	for (int i=0; i < m_projs.size(); ++i)
	{
		m_projs[i] = projs[i];
	}

}

void CShapefromSilhouettes::boundingBox(float x_min, float x_max, float y_min,
	float y_max, float z_min, float z_max, ushort nx, ushort ny, ushort nz)
{
	m_box_x[0] = x_min;
	m_box_x[1] = x_max;

	m_box_y[0] = y_min;
	m_box_y[1] = y_max;

	m_box_z[0] = z_min;
	m_box_z[1] = z_max;

	m_nx = nx;
	m_ny = ny;
	m_nz = nz;

	std::vector<float> voxel3Dx, voxel3Dy, voxel3Dz;
	linspace(0, m_nx, m_nx, voxel3Dx);
	linspace(0, m_ny, m_ny, voxel3Dy);
	linspace(0, m_nz, m_nz, voxel3Dz);

	unsigned int N = nx * ny * nz;
	m_voxels.resize(N);
	m_volume.resize(N, 0);
	auto v = m_voxels.begin();
	for (int i=0; i<nx; ++i)
	{
		for (int j=0; j<ny; ++j)
		{
			for (int k=0; k<nz; ++k)
			{
				*v = Mat::ones(4, 1, CV_32FC1);
				//auto& v = m_voxels[k + j * nz + i * (nz * ny)];
				v->at<float>(0, 0) = voxel3Dx[i];
				v->at<float>(1, 0) = voxel3Dy[j];
				v->at<float>(2, 0) = voxel3Dz[k];

				++v;
			}
		}
	}

	// Define transformation from volume to world coordinates.
	m_T = Mat::eye(4, 4, CV_32FC1);
	m_T.at<float>(0, 3) = m_box_x[0];
	m_T.at<float>(1, 3) = m_box_y[0];
	m_T.at<float>(2, 3) = m_box_z[0];

	Mat D = Mat::eye(4, 4, CV_32FC1);
	D.at<float>(0, 0) = (m_box_x[1] - m_box_x[0]) / m_nx;
	D.at<float>(1, 1) = (m_box_y[1] - m_box_y[0]) / m_ny;
	D.at<float>(2, 2) = (m_box_z[1] - m_box_z[0]) / m_nz;

	m_T = m_T * D;

	// flip y and z axes
	Mat F = Mat::zeros(4, 4, CV_32FC1);
	F.at<float>(0, 0) = 1;
	F.at<float>(1, 2) = 1;
	F.at<float>(2, 1) = -1;
	F.at<float>(3, 3) = 1;

	m_T = F * m_T;

	// subtract 1 for one - based indices
	Mat S = Mat::eye(4, 4, CV_32FC1);
	S.at<float>(0, 3) = -1;
	S.at<float>(1, 3) = -1;
	S.at<float>(2, 3) = -1;
	
	m_T = m_T * S;
}

void CShapefromSilhouettes::linspace(float minv, float maxv, ushort n, FloatArray& v)
{
	assert(minv < maxv && n > 1);
	float step_ = (maxv - minv) / (n-1);
	v.resize(n);
	float s = minv;
	for (auto& e : v)
	{
		e = s;
		s += step_;
	}
}

std::vector<cv::Point3f>& CShapefromSilhouettes::visualHull()
{
	int r{ 0 }, c{ 0 };
	for (int n = 0; n<m_sils.size(); ++n)
	{
#pragma omp parallel for  private(r, c)
		for (int i=0; i<m_nx*m_ny*m_nz; ++i)
		{
			Mat point2D = m_projs[n] * m_T * m_voxels[i];

			c = ceil(point2D.at<float>(0, 0) / point2D.at<float>(2, 0));
			r = ceil(point2D.at<float>(1, 0) / point2D.at<float>(2, 0));

			if (m_sils[n].at<uchar>(r, c) == 255) 
			{
				++m_volume[i];
			}
		}
	}

	unsigned int num_pts = 0;
	for (auto v : m_volume)
	{
		if (v > m_volumeThreshold)
			++num_pts;
	}
	m_PointCloud.resize(num_pts);

	auto pts = m_PointCloud.begin();
	auto  v   = m_voxels.begin();
	for (int i=0; i<m_voxels.size(); ++i, ++v)
	{
		if (m_volume[i] > m_volumeThreshold)
		{
			pts->x = v->at<float>(0, 0);
			pts->y = v->at<float>(1, 0);
			pts->z = v->at<float>(2, 0);
			
			++pts;
		}
	}
	return m_PointCloud;
}
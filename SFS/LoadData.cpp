#include "LoadData.h"


#include "LoadImage.h"
#include "LoadProjectMatrix.h"

CLoadData::CLoadData()
{
	
}


CLoadData::~CLoadData()
{
	if (!m_proj_img.empty())
		m_proj_img.clear();
}

bool CLoadData::Load(const string& imgfile, const string& projfile)
{
	cv::Mat img = CLoadImage::Load(imgfile);
	if (img.empty())
	{
		std::cerr << "Load img failed: "
			<< __FILE__ << ":" << __LINE__ << imgfile << std::endl;
		return false;
	}

	cv::Mat proj = CLoadProjectMatrix::Load(projfile);
	if (proj.empty())
	{
		std::cerr << "Load proj failed: "
			<< __FILE__ << ":" << __LINE__ << projfile << std::endl;

		return false;
	}

	m_proj_img.push_back(std::make_pair(img, proj));

	return true;
}
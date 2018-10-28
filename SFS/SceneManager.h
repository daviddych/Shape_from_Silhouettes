#pragma once
namespace pclViewer
{
	class CSceneManager
	{
	public:
		CSceneManager();
		~CSceneManager();

		void getMPV();
		int  project(double objx, double objy, double objz, double &winx, double &winy, double &winz);
		int  unproject(double winx, double winy, double winz, double &objx, double &objy, double &objz);

		int m_viewport[4];
		double m_modelmatrix[16];
		double m_projmatrix[16];
	};

}
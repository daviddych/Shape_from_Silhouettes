#include "SceneManager.h"
#include "GL/glew.h"

namespace pclViewer
{
	CSceneManager::CSceneManager()
	{
	}


	CSceneManager::~CSceneManager()
	{
	}


	void CSceneManager::getMPV()
	{
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelmatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, m_projmatrix);
		//printf("m_viewport: %d  %d  %d  %d\n", m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		//printf("m_modelmatrix: \n\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n", m_modelmatrix[0], m_modelmatrix[1], m_modelmatrix[2], m_modelmatrix[3], 
		//	m_modelmatrix[4], m_modelmatrix[5], m_modelmatrix[6], m_modelmatrix[7],
		//	m_modelmatrix[8], m_modelmatrix[9], m_modelmatrix[10], m_modelmatrix[11], 
		//	m_modelmatrix[12], m_modelmatrix[13], m_modelmatrix[14], m_modelmatrix[15] );
		//printf("m_projmatrix: \n\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n", m_projmatrix[0], m_projmatrix[1], m_projmatrix[2], m_projmatrix[3],
		//	m_projmatrix[4], m_projmatrix[5], m_projmatrix[6], m_projmatrix[7],
		//	m_projmatrix[8], m_projmatrix[9], m_projmatrix[10], m_projmatrix[11],
		//	m_projmatrix[12], m_projmatrix[13], m_projmatrix[14], m_projmatrix[15]);
	}

	int  CSceneManager::project(double objx, double objy, double objz, double &winx, double &winy, double &winz)
	{
		return gluProject(objx, objy, objz, m_modelmatrix, m_projmatrix, m_viewport, &winx, &winy, &winz);
	}

	int  CSceneManager::unproject(double winx, double winy, double winz, double &objx, double &objy, double &objz)
	{
		return gluUnProject(winx, winy, winz, m_modelmatrix, m_projmatrix, m_viewport, &objx, &objy, &objz);
	}
}
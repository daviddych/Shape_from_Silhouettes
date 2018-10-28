#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "PointCloud.h"
#include "SceneManager.h"

namespace pclViewer
{
	class CDataViewer
	{
		using Point3f = cv::Point3f;
	public:
		CDataViewer();
		~CDataViewer();

		// set the z of camera.
		void setCameraDistance(float camDist = 1.0f);
		// Initialize the GL scene params.
		void reSet(int argc, char**argv);
		// show point cloud, if reset=true, initialize the GL scene params.
		void imShow(std::vector<cv::Point3f>& cloudpoints);

	private:
		static void displayCB();
		static void reshapeCB(int w, int h);
		static void keyboardCB(unsigned char key, int x, int y);
		static void mouseCB(int button, int state, int x, int y);
		static void mouseMotionCB(int x, int y);


		static Point3f s_obj_center;
		static bool s_mouseLeftDown;
		static bool s_mouseRightDown;
		static bool s_mouseMiddleDown;
		static float s_mouseX, s_mouseY;
		static float s_cameraDistance;
		static float s_cameraAngleX;
		static float s_cameraAngleZ;
		static bool s_select_model;
		static Point3f s_select_pt;
		static pclViewer::CPointCloud*  s_pointcloud;
		static pclViewer::CSceneManager s_scenemanager;
	};
}



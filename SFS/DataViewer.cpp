#include "DataViewer.h"
#include "PointCloud.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <algorithm>

namespace pclViewer
{
	float g_z = -1.0f;
	cv::Point3f CDataViewer::s_obj_center;
	bool CDataViewer::s_mouseLeftDown = false;
	bool CDataViewer::s_mouseRightDown = false;
	bool CDataViewer::s_mouseMiddleDown = false;
	float CDataViewer::s_mouseX;
	float CDataViewer::s_mouseY;

	float CDataViewer::s_cameraDistance = -1.0f;
	float CDataViewer::s_cameraAngleX = -75.0f;
	float CDataViewer::s_cameraAngleZ = .0f;
	bool  CDataViewer::s_select_model = false;
	cv::Point3f CDataViewer::s_select_pt;

	pclViewer::CPointCloud*  CDataViewer::s_pointcloud = nullptr;
	pclViewer::CSceneManager CDataViewer::s_scenemanager;

	CDataViewer::CDataViewer()
	{
	}


	CDataViewer::~CDataViewer()
	{
	}

	void CDataViewer::setCameraDistance(float camDist)
	{
		s_cameraDistance = camDist;
		g_z = camDist;
	}
	void CDataViewer::reSet(int argc, char**argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(640, 480);
		glutInitWindowPosition(250, 150);
		glutCreateWindow("SFS: ×ó¼üÐý×ª£¬¹öÂÖËõ·Å£¬ÓÒ¼üÊ°È¡");
		printf("²Ù×÷·½·¨£º ×ó¼üÐý×ª£¬¹öÂÖËõ·Å£¬ÓÒ¼üÊ°È¡\n");

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			printf("GLEW is not initialized!");
		}
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glDisable(GL_DEPTH_TEST);

		glutDisplayFunc(displayCB);
		glutReshapeFunc(reshapeCB);
		glutKeyboardFunc(keyboardCB);
		glutMouseFunc(mouseCB);
		glutMotionFunc(mouseMotionCB);
	}

	void CDataViewer::imShow(std::vector<cv::Point3f>& xyz)
	{
		Point3f leftbottom, rightup;
		[&xyz, &leftbottom, &rightup]() {
			std::sort(xyz.begin(), xyz.end(), [](const Point3f& d1, const Point3f& d2) { return d1.x > d2.x; });
			rightup.x = xyz[0].x;
			leftbottom.x = xyz[xyz.size() - 1].x;
		}();

		[&xyz, &leftbottom, &rightup]() {
			std::sort(xyz.begin(), xyz.end(), [](const Point3f& d1, const Point3f& d2) { return d1.y > d2.y; });
			rightup.y = xyz[0].y;
			leftbottom.y = xyz[xyz.size() - 1].y;
		}();

		[&xyz, &leftbottom, &rightup]() {
			std::sort(xyz.begin(), xyz.end(), [](const Point3f& d1, const Point3f& d2) { return d1.z > d2.z; });
			rightup.z = xyz[0].z;
			leftbottom.z = xyz[xyz.size() - 1].z;
		}();

		s_obj_center = (rightup + leftbottom) / 2.0f;
		std::for_each(xyz.begin(), xyz.end(), [](Point3f& pt) { pt -= s_obj_center; });

		Point3f boxr = rightup - leftbottom;
		s_cameraDistance = -std::max(boxr.x, std::max(boxr.y, boxr.z))*1.73;
		g_z = s_cameraDistance;

		s_pointcloud = new pclViewer::CPointCloud(xyz);
		glutMainLoop();
	}

	void CDataViewer::displayCB()
	{
		glClearColor(0.0235f, 0.2588f, 0.3843f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); // GL_PROJECTION
		glLoadIdentity();
		if (s_pointcloud != nullptr)
		{
			glTranslatef(0, 0, s_cameraDistance);
			glRotatef(s_cameraAngleX, 1, 0, 0);   // pitch
			glRotatef(s_cameraAngleZ, 0, 0, 1);   // 

			s_scenemanager.getMPV();
			if (s_pointcloud != nullptr)
			{
				s_pointcloud->Render();
			}

			if (s_select_model)
			{
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPointSize(10);
				glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(s_select_pt.x, s_select_pt.y, s_select_pt.z);
				glEnd();
				glPopAttrib();
			}
		}
		glutSwapBuffers();
	}

	void CDataViewer::reshapeCB(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, (float)(w) / h, 0.01f, 100000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void CDataViewer::keyboardCB(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27: 
			exit(0);
			break;
		case  'w':
			s_cameraAngleZ -= -1.0f;
			break;
		case 'x':
			s_cameraAngleZ += -1.0f;
			break;
		case 'a':
		case 'A':
			s_cameraAngleX -= 1.0f;
			break;
		case 'd':
		case 'D':
			s_cameraAngleX += 1.0f;
			break;

		case 'r':
		case 'R':
			s_cameraAngleX = -75.0f;
			s_cameraAngleZ = 0;
			s_cameraDistance = g_z;
			break;
		}
		glutPostRedisplay();
	}

	void CDataViewer::mouseCB(int button, int state, int x, int y)
	{
		if (s_pointcloud == nullptr)
			return;

		s_mouseX = float(x);
		s_mouseY = float(y);

		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				s_mouseLeftDown = true;
			}
			else if (state == GLUT_UP)
			{
				s_mouseLeftDown = false;
			}
		}
		else if (GLUT_MIDDLE_BUTTON == button && state == GLUT_DOWN)
		{
			// Controls whether to highlight the selected point.
			s_select_model = !s_select_model;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			double min_dist = std::numeric_limits<double>::max(), dist, winX, winY, winZ, dx, dy; // , objx, objy, objz
			size_t best_pt_id = 0;
			std::vector<Point3f> xyz = s_pointcloud->get_xyz();
			for (size_t i = 0; i < xyz.size(); ++i)
			{
				if (GL_TRUE == s_scenemanager.project(xyz[i].x, xyz[i].y, xyz[i].z, winX, winY, winZ))
				{
					if (winZ > 0.0f && winZ < 1.0f)
					{
						winY = s_scenemanager.m_viewport[3] - winY - 7;
						dx = x - winX; dy = y - winY;
						dist = dx * dx + dy * dy;
						if (min_dist > dist)
						{
							min_dist = dist;
							best_pt_id = i;
						}
					}
				}
			}
			s_select_pt.x = xyz[best_pt_id].x; s_select_pt.y = xyz[best_pt_id].y; s_select_pt.z = xyz[best_pt_id].z;
			printf("Picked Point: <%f, %f, %f>\n", s_select_pt.x+s_obj_center.x, s_select_pt.y + s_obj_center.y, s_select_pt.z + s_obj_center.z);
			s_select_model = true;
			glutPostRedisplay();
		}

		else if (state == GLUT_UP && button == 4)
		{
			s_cameraDistance += s_pointcloud->getBox()->_length.z * 0.2f;
			glutPostRedisplay();
		}
		else if (state == GLUT_DOWN && button == 3)
		{
			s_cameraDistance -= s_pointcloud->getBox()->_length.z * 0.2f;
			glutPostRedisplay();
		}
		printf("Translate z = %f\n", s_cameraDistance);
	}

	void CDataViewer::mouseMotionCB(int x, int y)
	{
		if (s_mouseLeftDown)
		{
			s_cameraAngleZ += (x - s_mouseX);
			s_cameraAngleX += (y - s_mouseY);
			s_mouseX = float(x);
			s_mouseY = float(y);

			printf("Rotate<x, y> = <%f, %f>\n", s_cameraAngleX, s_cameraAngleZ);
		}
		if (s_mouseRightDown)
		{
			s_cameraDistance += (y - s_mouseY) * 0.2f;

			s_mouseY = float(y);
		}

		glutPostRedisplay();
	}
}
#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GraphicObj.h"

namespace pclViewer
{
	class CPointCloud : public CGraphicObj
	{
		using Point3f = cv::Point3f;
		using Point3fArray = std::vector<Point3f>;

	public:
		CPointCloud(const Point3fArray& xyz, Point3fArray& rgb = Point3fArray());
		~CPointCloud();

		void Render();
		void Render_VBO();
		void Render_VertexArray();

		Point3fArray& get_xyz() {
			return m_xyz;
		}

	private:
		void CreateVBO();
		void Box();

		Point3fArray m_xyz;
		Point3fArray m_rgb;
	};
}

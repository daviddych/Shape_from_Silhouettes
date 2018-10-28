#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

namespace pclViewer {
	class CBoundingBox 
	{
		using Point3f = cv::Point3f;
		using Point3fArray = std::vector<Point3f>;

	public:
		CBoundingBox(const Point3fArray& xyz);
		CBoundingBox(Point3f lb, Point3f rt);
		~CBoundingBox();

		void Render();

		Point3f _leftbottom;
		Point3f _rightup;
		Point3f _center;
		Point3f _length;

	private:
		void init_graphic();

		unsigned int _vbo[2];
		unsigned int _vaoHandle;
		static unsigned char s_indices[24];
	};
}


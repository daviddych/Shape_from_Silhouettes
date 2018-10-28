#include "BoundingBox.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>  

namespace pclViewer
{
	unsigned char CBoundingBox::s_indices[24] = { 0,1,1,2,2,3,3,0,5,6,6,7,7,4,4,5,6,1,7,2,4,3,5,0 };

	CBoundingBox::CBoundingBox(const Point3fArray& xyz) : _vaoHandle(0)
	{
		init_graphic();
	}

	CBoundingBox::CBoundingBox(Point3f lb, Point3f rt) : _vaoHandle(0)
	{
		_leftbottom.x = lb.x; _leftbottom.y = lb.y; _leftbottom.z = lb.z;
		_rightup.x = rt.x;    _rightup.y = rt.y;    _rightup.z = rt.z;

		_center.x = (_leftbottom.x + _rightup.x) / 2.0f;
		_center.y = (_leftbottom.y + _rightup.y) / 2.0f;
		_center.z = (_leftbottom.z + _rightup.z) / 2.0f;

		_length.x = (_rightup.x - _leftbottom.x);
		_length.y = (_rightup.y - _leftbottom.y);
		_length.z = (_rightup.z - _leftbottom.z);

		init_graphic();
	}

	void CBoundingBox::Render()
	{
		if (_vaoHandle == 0) return;
		
		glBindVertexArray(_vaoHandle);
		glLineWidth(1);
		glDrawElements(GL_LINES,  24, GL_UNSIGNED_BYTE, s_indices);
		glBindVertexArray(0);

		return;
	}


	CBoundingBox::~CBoundingBox()
	{
		glDeleteBuffers(2, _vbo);
		glDeleteVertexArrays(1, &_vaoHandle);
	}

	void CBoundingBox::init_graphic()
	{
		// cube ///////////////////////////////////////////////////////////////////////
		//    v6----- v5
		//   /|      /|
		//  v1------v0|
		//  | |     | |
		//  | |v7---|-|v4
		//  |/      |/
		//  v2------v3

		float x1(_leftbottom.x), y1(_leftbottom.y), z1(_leftbottom.z);
		float x2(_rightup.x), y2(_rightup.y), z2(_rightup.z);

		// vertex coords array
		float vertices[] = { x2,y1,z2,  x1,y1,z2,   x1,y1,z1,    x2,y1,z1,    // v0-v1-v2-v3
			x2,y2,z1,    x2,y2,z2,   x1,y2,z2,   x1,y2,z1 };  // v4-v5-v6-v7

		// normal array
		//float normals[] = { 0,0,1,  0,0,1,  0,0,1,  0,0,1,  // v0-v1-v2-v3
		//	0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1 };        // v4-v5-v6-v7

		// color array
		float colors[] = { 1,1,1,  1,1,0,  1,0,0,  1,0,1, // v0-v1-v2-v3
			0,0,1,  0,1,1,  0,1,0,  1,1,1 };             // v4-v5-v6-v7


	// index array of vertex array for glDrawElements()
	// Notice the indices are listed straight from beginning to end as exactly
	// same order of vertex array without hopping, because of different normals at
	// a shared vertex. For this case, glDrawArrays() and glDrawElements() have no
	// difference.

	    // Allocate Vertex Array Object
		glGenVertexArrays(1, &_vaoHandle);
		glGenBuffers(2, _vbo);

		glBindVertexArray(_vaoHandle);

		// vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		// color buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, 0);
		
		glBindVertexArray(0);
	}
}
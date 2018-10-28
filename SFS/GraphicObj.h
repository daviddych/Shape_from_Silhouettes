#pragma once

#include <gl/glew.h>
#include "BoundingBox.h"


namespace pclViewer
{
	class CGraphicObj
	{
	public:
		CGraphicObj();
		~CGraphicObj();

		virtual void Render() = 0;
		CBoundingBox* getBox();
	protected:
		CBoundingBox *m_box;
		GLuint        m_vbo[1];
	};
}
#include "GraphicObj.h"

namespace pclViewer
{
	CGraphicObj::CGraphicObj():m_box(nullptr)
	{
		m_vbo[0] = 0;
	}


	CGraphicObj::~CGraphicObj()
	{
		if (m_vbo[0] != 0)
			glDeleteBuffers(1, m_vbo);
	}
	CBoundingBox* CGraphicObj::getBox()
	{
		return m_box;
	}
}

#include "PointCloud.h"
#include <assert.h>
#include <algorithm>

namespace pclViewer
{
	CPointCloud::CPointCloud(const Point3fArray& xyz, Point3fArray& rgb)
	{
		if (!xyz.empty())
		{
			m_xyz.resize(xyz.size());
			std::copy(xyz.begin(), xyz.end(), m_xyz.begin());
			Box();

			if (!rgb.empty())
			{
				assert(xyz.size() == rgb.size());
				m_rgb.resize(rgb.size());
				std::copy(rgb.begin(), rgb.end(), m_rgb.begin());
			}

			CreateVBO();
			
		}
	}


	CPointCloud::~CPointCloud()
	{
	}

	void CPointCloud::Render_VertexArray()
	{
		if (m_rgb.empty())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, &m_xyz[0].x);
			glDrawArrays(GL_POINTS, 0, m_xyz.size());
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, &m_xyz[0].x);
			glColorPointer(3, GL_FLOAT, 0, &m_rgb[0].x);
			glDrawArrays(GL_POINTS, 0, m_xyz.size());
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		}
	}

	void CPointCloud::Render_VBO()
	{
		glDisable(GL_POINT_SMOOTH);
		if (m_vbo[0] >= 0)
		{
			if (m_rgb.empty())
			{
				glBindBufferARB(GL_ARRAY_BUFFER, m_vbo[0]);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, 0);
				glDrawArrays(GL_POINTS, 0, m_xyz.size());
				glDisableClientState(GL_VERTEX_ARRAY);
				glBindBufferARB(GL_ARRAY_BUFFER, 0);
			}
			else
			{
				glBindBufferARB(GL_ARRAY_BUFFER, m_vbo[0]);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, 0);
				glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(float) * 3 * m_xyz.size()));
				glDrawArrays(GL_POINTS, 0, m_xyz.size());
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glBindBufferARB(GL_ARRAY_BUFFER, 0);
			}
		}
		glEnable(GL_POINT_SMOOTH);
	}

	void CPointCloud::Render()
	{
		if (m_vbo[0] == 0)
			Render_VertexArray();
		else
			Render_VBO();

		if (m_box != nullptr)
			m_box->Render();

		return;
	}

	void CPointCloud::CreateVBO()
	{
		if (m_xyz.empty())
			return;

		glGenBuffers(1, m_vbo);

		if (m_rgb.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, m_xyz.size() * 3 * sizeof(float), &m_xyz[0].x, GL_STATIC_DRAW);
			GLenum glErr = glGetError();
			if (glErr != GL_NO_ERROR)
			{
				glGenBuffers(1, m_vbo);
				glDeleteBuffers(1, m_vbo);
				m_vbo[0] = 0;
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else
		{
			assert(m_xyz.size() == m_rgb.size());

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, m_xyz.size() * 3 * sizeof(float) * 2, 0, GL_STATIC_DRAW);
			GLenum glErr = glGetError();
			if (glErr != GL_NO_ERROR)
			{
				glGenBuffers(1, m_vbo);
				glDeleteBuffers(1, m_vbo);
				m_vbo[0] = 0;
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_xyz.size() * 3 * sizeof(float), &m_xyz[0].x);
			glBufferSubData(GL_ARRAY_BUFFER, m_xyz.size() * 3 * sizeof(float), m_rgb.size() * 3 * sizeof(float), &m_rgb[0].x);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	void CPointCloud::Box()
	{
		if (m_xyz.empty())
			return;

		float x_min = std::numeric_limits<float>::max();
		float x_max = -x_min;

		float y_min = x_min, z_min = x_min;
		float y_max = x_max, z_max = x_max;
		Point3f pt;
		for (auto iter = m_xyz.begin(); iter != m_xyz.end(); ++iter)
		{
			x_min = x_min < iter->x ? x_min : iter->x;
			x_max = x_max > iter->x ? x_max : iter->x;
			y_min = y_min < iter->y ? y_min : iter->y;
			y_max = y_max > iter->y ? y_max : iter->y;
			z_min = z_min < iter->z ? z_min : iter->z;
			z_max = z_max > iter->z ? z_max : iter->z;
		}
		Point3f lb, rt;
		lb.x = x_min; lb.y = y_min; lb.z = z_min;
		rt.x = x_max; rt.y = y_max; rt.z = z_max;
		m_box = new CBoundingBox(lb, rt);
	}
}
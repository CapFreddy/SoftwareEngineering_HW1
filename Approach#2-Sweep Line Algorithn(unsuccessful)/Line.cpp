#include "global.h"
#include "Line.h"
#include "Node.h"


Line::Line(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{
	if (m_x1 == m_x2)
	{
		m_vertical = true;
		m_parallel = false;
	}
	else
	{
		m_vertical = false;
		m_k = (double)(m_y1 - m_y2) / (m_x1 - m_x2);
		m_parallel = m_k == 0;
	}
}

Intercept Line::InterceptX()
{
	double x = (double)(m_x2 - m_x1) / (m_y1 - m_y2) * m_y1 + m_x1;
	return Intercept(x, 0, this);

}

vector<Node> Line::Intersect(Line line)
{
	cout << m_x1 << ' ' << m_y1 << endl;
	cout << line.m_x2 << ' ' << m_y2 << endl;
	cout << m_k << ' '<< line.m_k;

	double x, y;
	vector<Node> intersections;

	if (!parallelWith(line))

	{
		if (this->m_vertical)
		{
			x = m_x1;
			y = line.m_k * (m_x1 - line.m_x1) + line.m_y1;
		}
		else if (line.m_vertical)
		{
			x = line.m_x1;
			y = m_k * (line.m_x1 - m_x1) + m_y1;
		}
		else
		{
			x = (double)(m_k * m_x1 - line.m_k * line.m_x1 + line.m_y1 - m_y1) / (m_k - line.m_k);
			y = (double)m_k * (line.m_k * (m_x1 - line.m_x1) + line.m_y1 - m_y1) / (m_k - line.m_k) + m_y1;
		}
		intersections.push_back(Node(x, y));
	}
	return intersections;
}

bool Line::parallelWith(Line line)
{
	if (m_vertical || line.m_vertical)
	{
		return m_vertical && line.m_vertical;
	}
	return m_k == line.m_k;
}

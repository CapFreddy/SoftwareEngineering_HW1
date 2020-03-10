#pragma once
#include "global.h"

class Node;
class Intercept;
class Line
{
public:
	bool m_vertical, m_parallel;
	
	Line(int, int, int, int);

	Intercept InterceptX();

	vector<Node> Intersect(Line);

private:
	int m_x1, m_y1, m_x2, m_y2;
	double m_k;

	bool parallelWith(Line line);

};

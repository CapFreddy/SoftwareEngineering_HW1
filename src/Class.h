#include <set>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
using namespace std;

constexpr auto EPS = 1e-6;


class Node
{
public:
	double m_x, m_y;
	Node(double x, double y) : m_x(x), m_y(y) {}

	bool operator ==(const Node& obj) const
	{
		return fabs(m_x - obj.m_x) < 1e-6 && fabs(m_y - obj.m_y) < 1e-6;
	}
};

struct NodePointerHasher
{
	size_t operator()(const Node* node) const noexcept
	{
		return hash<double>{}(node->m_x + node->m_y);
	}
};

struct NodePointerComparator
{
	bool operator()(const Node* node1, const Node* node2) const noexcept
	{
		return fabs(node1->m_x - node2->m_x) < EPS && fabs(node1->m_y - node2->m_y) < EPS;
	}
};


class Circle
{
public:
	int m_x, m_y, m_r;

	Circle(int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}

	vector<Node*> Intersect(Circle* circle)
	{
		double d, a, h, x2, y2, x3, y3, x4, y4;
		Node* intersection;
		vector<Node*> intersections;

		d = sqrt((circle->m_x - m_x) * (circle->m_x - m_x) + (circle->m_y - m_y) * (circle->m_y - m_y));
		if (intersects(circle, d))
		{
			a = ((double)m_r * m_r - (double)circle->m_r * circle->m_r + d * d) / (2 * d);
			h = sqrt((double)m_r * m_r - a * a);
			x2 = m_x + a * ((double)circle->m_x - m_x) / d;
			y2 = m_y + a * ((double)circle->m_y - m_y) / d;
			
			if (d < 1e-6)
			{
				intersection = new Node(x2, y2);
				intersections.push_back(intersection);
			}
			else 
			{
				x3 = x2 + h * ((double)circle->m_y - m_y) / d;
				y3 = y2 - h * ((double)circle->m_x - m_x) / d;
				x4 = x2 - h * ((double)circle->m_y - m_y) / d;
				y4 = y2 + h * ((double)circle->m_x - m_x) / d;
				
				intersection = new Node(x3, y3);
				intersections.push_back(intersection);
				intersection = new Node(x4, y4);
				intersections.push_back(intersection);
			}
		}
		return intersections;
	}

	int getX() { return m_x; }

	int getY() { return m_y; }

	int getR() { return m_r; }

private:
	bool intersects(Circle* circle, double d)
	{
		if (d > (double)m_r + circle->m_r)
		{
			return false;
		}
		return d >= abs(m_r - circle->m_r);
	}
};


class Line
{
public:
	Line(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
	{
		m_vertical = m_x1 == m_x2;
		m_k = m_vertical ? 0 : ((double)m_y1 - m_y2) / ((double)m_x1 - m_x2);
	}

	vector<Node*> IntersectLine(Line* line)
	{
		double x, y;
		Node* intersection;
		vector<Node*> intersections;

		if (!parallels(line))
		{
			if (m_vertical)
			{
				x = m_x1;
				y = line->m_k * ((double)m_x1 - line->m_x1) + line->m_y1;
			}
			else if (line->m_vertical)
			{
				x = line->m_x1;
				y = m_k * ((double)line->m_x1 - m_x1) + m_y1;
			}
			else
			{
				x = (m_k * m_x1 - line->m_k * line->m_x1 + line->m_y1 - m_y1) / (m_k - line->m_k);
				y = m_k * (line->m_k * ((double)m_x1 - line->m_x1) + (double)line->m_y1 - m_y1) / (m_k - line->m_k) + m_y1;
			}
			intersection = new Node(x, y);
			intersections.push_back(intersection);
		}
		return intersections;
	}

	vector<Node*> IntersectCircle(Circle* circle)
	{
		vector<Node*> intersections;
		Node* intersection;

		int dx = m_x2 - m_x1;
		int dy = m_y2 - m_y1;
		double dr = sqrt(dx * dx + dy * dy);
		int D = (m_x1 - circle->m_x) * (m_y2 - circle->m_y) - (m_x2 - circle->m_x) * (m_y1 - circle->m_y);

		double delta = circle->m_r * circle->m_r * dr * dr - D * D;
		if (!(delta <= -EPS))
		{
			if (fabs(delta) < EPS)
			{
				double x = (D * dy) / (dr * dr) + circle->m_x;
				double y = (-D * dx) / (dr * dr) + circle->m_y;
				intersection = new Node(x, y);
				intersections.push_back(intersection);
			}
			else
			{
				double x1 = (D * dy + copysign(1, dy) * dx * sqrt(delta)) / (dr * dr) + circle->m_x;
				double y1 = (-D * dx + fabs(dy) * sqrt(delta)) / (dr * dr) + circle->m_y;
				intersection = new Node(x1, y1);
				intersections.push_back(intersection);

				double x2 = (D * dy - copysign(1, dy) * dx * sqrt(delta)) / (dr * dr) + circle->m_x;
				double y2 = (-D * dx - fabs(dy) * sqrt(delta)) / (dr *  dr) + circle->m_y;
				intersection = new Node(x2, y2);
				intersections.push_back(intersection);	
			}
		}
		return intersections;
	}

private:
	int m_x1, m_y1, m_x2, m_y2;
	bool m_vertical;
	double m_k;

	bool parallels(Line* line)
	{
		if (m_vertical || line->m_vertical)
		{
			return m_vertical && line->m_vertical;
		}
		return fabs(m_k - line->m_k) < 1e-6;
	}
};


class BruteForce
{
public:
	void Solve(string in_file, string out_file)
	{
		Input(in_file);
		IntersectLLC();
		IntersectCC();
		Output(out_file);
		return;
	}

	int getIntersection(string in_file)
	{
		Input(in_file);
		IntersectLLC();
		IntersectCC();
		check_intersections();
		return m_allIntersections.size();
	}

private:
	vector<Line*> m_allLines;
	vector<Circle*> m_allCircles;
	unordered_set<Node*, NodePointerHasher, NodePointerComparator> m_allIntersections;

	void Input(string in_file)
	{
		char geoType;
		int N;
		Line* newLine;
		Circle* newCircle;
		ifstream ifile;
		
		ifile.open(in_file, ios::in);
		ifile >> N;
		while (N--)
		{
			ifile >> geoType;
			if (geoType == 'L')
			{
				int x1, y1, x2, y2;
				
				ifile >> x1 >> y1 >> x2 >> y2;
				newLine = new Line(x1, y1, x2, y2);
				m_allLines.push_back(newLine);
			}
			else if (geoType == 'C')
			{
				int x, y, r;

				ifile >> x >> y >> r;
				newCircle = new Circle(x, y, r);
				m_allCircles.push_back(newCircle);
			}
		}
		ifile.close();
		return;
	}

	void IntersectLLC()
	{
		int i, j, k;
		int lineNum = m_allLines.size();
		int circleNum = m_allCircles.size();
		int intersectionNum;
		Line* lineOut;
		Line* lineIn;
		Circle* circleIn;
		vector<Node*> intersections;


		for (i = 0; i < lineNum; i++)
		{
			lineOut = m_allLines[i];
			for (j = i + 1; j < lineNum; j++)
			{
				lineIn = m_allLines[j];
				intersections = lineOut->IntersectLine(lineIn);
				intersectionNum = intersections.size();
				for (k = 0; k < intersectionNum; k++)
				{
					m_allIntersections.insert(intersections[k]);
				}
			}
			for (j = 0; j < circleNum; j++)
			{
				circleIn = m_allCircles[j];
				intersections = lineOut->IntersectCircle(circleIn);
				intersectionNum = intersections.size();
				for (k = 0; k < intersectionNum; k++)
				{
					m_allIntersections.insert(intersections[k]);
				}
			}
		}
		return;
	}

	void IntersectCC()
	{
		int i, j, k;
		int circleNum = m_allCircles.size();
		int intersectionNum;
		Circle* circleOut;
		Circle* circleIn;
		vector<Node*> intersections;

		for (i = 0; i < circleNum; i++)
		{
			circleOut = m_allCircles[i];
			for (j = i + 1; j < circleNum; j++)
			{
				circleIn = m_allCircles[j];
				intersections = circleOut->Intersect(circleIn);
				intersectionNum = intersections.size();
				for (k = 0; k < intersectionNum; k++)
				{
					m_allIntersections.insert(intersections[k]);
				}
			}
		}
		return;
	}

	void Output(string out_file)
	{
		ofstream ofile;
		ofile.open(out_file, ios::out);
		ofile << m_allIntersections.size();
		ofile.close();
		return;
	}

	void check_intersections()
	{
		int i;
		Node* intersection;

		for (unordered_set<Node*>::iterator iter = m_allIntersections.begin(); iter != m_allIntersections.end(); iter++)
		{
			intersection = *iter;
			cout << intersection->m_x << ' ' << intersection->m_y << endl;
		}
		return;
	}
};
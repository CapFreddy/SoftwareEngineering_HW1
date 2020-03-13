#include <set>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
using namespace std;

double constexpr EPS = 1e-10;


class Node
{
public:
	double m_x, m_y;
	Node(double x, double y) : m_x(x), m_y(y) {}
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
		return fabs(node1->m_x - node2->m_x) <= EPS && fabs(node1->m_y - node2->m_y) <= EPS;
	}
};


class Circle
{
public:
	long long m_x, m_y, m_r;

	Circle(long long x, long long y, long long r) : m_x(x), m_y(y), m_r(r) {}

	vector<Node*> Intersect(Circle* circle)
	{
		vector<Node*> intersections;

		double dis = sqrt((circle->m_x - m_x) * (circle->m_x - m_x) + (circle->m_y - m_y) * (circle->m_y - m_y));
		if (!(m_r + circle->m_r < dis || fabs(m_r - circle->m_r) > dis))
		{
			double a = (m_r * m_r - circle->m_r * circle->m_r + dis * dis) / (2 * dis);
			double h = sqrt(m_r * m_r - a * a);
			double x = m_x + a * (circle->m_x - m_x) / dis;
			double y = m_y + a * (circle->m_y - m_y) / dis;
			
			if (dis < EPS)
			{
				Node* intersection = new Node(x, y);
				intersections.push_back(intersection);
			}
			else 
			{
				double x1 = x + h * (circle->m_y - m_y) / dis;
				double y1 = y - h * (circle->m_x - m_x) / dis;
				Node* intersection = new Node(x1, y1);
				intersections.push_back(intersection);

				double x2 = x - h * (circle->m_y - m_y) / dis;
				double y2 = y + h * (circle->m_x - m_x) / dis;
				Node* intersection = new Node(x2, y2);
				intersections.push_back(intersection);
			}
		}
		return intersections;
	}
};


class Line
{
public:
	long long m_x1, m_y1, m_x2, m_y2;
	long long m_xdiff, m_ydiff, m_det;
	Line(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
	{
		m_xdiff = m_x1 - m_x2;
		m_ydiff = m_y1 - m_y2;
		m_det = m_x1 * m_y2 - m_x2 * m_y1;
	}

	vector<Node*> IntersectLine(Line* line)
	{
		vector<Node*> intersections;

		long long den = m_xdiff * line->m_ydiff - m_ydiff * line->m_xdiff;
		if (den != 0)
		{
			long long xmol = m_det * line->m_xdiff - m_xdiff * line->m_det;
			long long ymol = m_det * line->m_ydiff - m_ydiff * line->m_det;
			double x = (double)xmol / (double)den;
			double y = (double)ymol / (double)den;
			Node* intersection = new Node(x, y);
			intersections.push_back(intersection);
		}
		return intersections;
	}

	vector<Node*> IntersectCircle(Circle* circle)
	{
		vector<Node*> intersections;

		double dis = sqrt(m_xdiff * m_xdiff + m_ydiff * m_ydiff);
		double det = (m_x1 - circle->m_x) * (m_y2 - circle->m_y) - (m_x2 - circle->m_x) * (m_y1 - circle->m_y);
		double delta = circle->m_r * circle->m_r * dis * dis - det * det;
		if (!(delta < -EPS))
		{
			// delta >= 0
			if (fabs(delta) <= EPS)
			{
				// delta == 0
				double x = (det * -m_ydiff) / (dis * dis) + circle->m_x;
				double y = (det * m_xdiff) / (dis * dis) + circle->m_y;
				Node* intersection = new Node(x, y);
				intersections.push_back(intersection);
			}
			else
			{
				// delta > 0
				double x1 = (det * -m_ydiff + copysign(1, -m_ydiff) * -m_xdiff * sqrt(delta)) / (dis * dis) + circle->m_x;
				double y1 = (det * m_xdiff + fabs(m_ydiff) * sqrt(delta)) / (dis * dis) + circle->m_y;
				Node* intersection = new Node(x1, y1);
				intersections.push_back(intersection);

				double x2 = (det * -m_ydiff - copysign(1, -m_ydiff) * -m_xdiff * sqrt(delta)) / (dis * dis) + circle->m_x;
				double y2 = (det * m_xdiff - fabs(m_ydiff) * sqrt(delta)) / (dis *  dis) + circle->m_y;
				intersection = new Node(x2, y2);
				intersections.push_back(intersection);	
			}
		}
		return intersections;
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
				long long x1, y1, x2, y2;
				
				ifile >> x1 >> y1 >> x2 >> y2;
				newLine = new Line(x1, y1, x2, y2);
				m_allLines.push_back(newLine);
			}
			else if (geoType == 'C')
			{
				long long x, y, r;

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
		Node* intersection;

		for (unordered_set<Node*>::iterator iter = m_allIntersections.begin(); iter != m_allIntersections.end(); iter++)
		{
			intersection = *iter;
			cout << intersection->m_x << ' ' << intersection->m_y << endl;
		}
		return;
	}
};
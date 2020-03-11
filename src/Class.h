#include <set>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;


class Node
{
public:
	Node(double x, double y) : m_x(x), m_y(y) {}

	bool operator ==(const Node& obj) const
	{
		return abs(m_x - obj.m_x) < 1e-6 && abs(m_y - obj.m_y) < 1e-6;
	}

	double getX() const { return m_x; }

	double getY() const { return m_y; }

private:
	double m_x, m_y;
};
namespace std
{
	template<>
	struct hash<Node>
	{
		size_t operator()(const Node& obj) const
		{
			return hash<long long>()(llround(obj.getX())) + hash<long long>()(llround(obj.getY()));
		}
	};
}


class Circle
{
public:
	Circle(int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}

	vector<Node> Intersect(Circle circle)
	{
		double d, a, h, x2, y2, x3, y3, x4, y4;
		vector<Node> intersections;

		d = sqrt((circle.m_x - m_x) * (circle.m_x - m_x) + (circle.m_y - m_y) * (circle.m_y - m_y));
		if (intersects(circle, d))
		{
			a = ((double)m_r * m_r - (double)circle.m_r * circle.m_r + d * d) / (2 * d);
			h = sqrt((double)m_r * m_r - a * a);
			x2 = m_x + a * ((double)circle.m_x - m_x) / d;
			y2 = m_y + a * ((double)circle.m_y - m_y) / d;
			
			if (d < 1e-6)
			{
				Node intersection(x2, y2);
				intersections.push_back(intersection);
			}
			else 
			{
				x3 = x2 + h * ((double)circle.m_y - m_y) / d;
				y3 = y2 - h * ((double)circle.m_x - m_x) / d;
				x4 = x2 - h * ((double)circle.m_y - m_y) / d;
				y4 = y2 + h * ((double)circle.m_x - m_x) / d;
				Node intersection1(x3, y3);
				Node intersection2(x4, y4);
				intersections.push_back(intersection1);
				intersections.push_back(intersection2);
			}
		}
		return intersections;
	}

	int getX() { return m_x; }

	int getY() { return m_y; }

	int getR() { return m_r; }

private:
	int m_x, m_y, m_r;

	bool intersects(Circle circle, double d)
	{
		if (d > (double)m_r + circle.m_r)
		{
			return false;
		}
		return d >= abs(m_r - circle.m_r);
	}
};


class Line
{
public:
	Line(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
	{
		m_vertical = m_x1 == m_x2;
		m_k = m_vertical ? 0 : (double)((double)m_y1 - m_y2) / ((double)m_x1 - m_x2);
	}

	bool parallel2X() {
		return m_y1 == m_y2;
	}

	vector<Node> IntersectLine(Line line)
	{
		double x, y;
		vector<Node> intersections;

		if (!parallels(line))
		{
			if (this->m_vertical)
			{
				x = m_x1;
				y = line.m_k * ((double)m_x1 - line.m_x1) + line.m_y1;
			}
			else if (line.m_vertical)
			{
				x = line.m_x1;
				y = m_k * ((double)line.m_x1 - m_x1) + m_y1;
			}
			else
			{
				x = (double)(m_k * m_x1 - line.m_k * line.m_x1 + line.m_y1 - m_y1) / (m_k - line.m_k);
				y = (double)m_k * (line.m_k * ((double)m_x1 - line.m_x1) + (double)line.m_y1 - m_y1) / (m_k - line.m_k) + m_y1;
			}
			intersections.push_back(Node(x, y));
		}
		return intersections;
	}

	vector<Node> IntersectCircle(Circle circle)
	{
		vector<Node> intersections;
		int baX = m_x2 - m_x1;
		int baY = m_y2 - m_y1;
		int caX = circle.getX() - m_x1;
		int caY = circle.getY() - m_y1;

		int a = baX * baX + baY * baY;
		int bBy2 = baX * caX + baY * caY;
		int c = caX * caX + caY * caY - circle.getR() * circle.getR();

		double pBy2 = (double)bBy2 / a;
		double q = (double)c / a;

		double disc = pBy2 * pBy2 - q;
		if (disc >= 0)
		{
			double tmpSqrt = sqrt(disc);
			double abScalingFactor1 = -pBy2 + tmpSqrt;
			double abScalingFactor2 = -pBy2 - tmpSqrt;

			intersections.push_back(Node(m_x1 - baX * abScalingFactor1, m_y1 - baY * abScalingFactor1));
			if (disc != 0)
			{
				intersections.push_back(Node(m_x1 - baX * abScalingFactor2, m_y1 - baY * abScalingFactor2));
			}
		}
		return intersections;
	}

private:
	int m_x1, m_y1, m_x2, m_y2;
	bool m_vertical;
	double m_k;

	bool parallels(Line line)
	{
		if (m_vertical || line.m_vertical)
		{
			return m_vertical && line.m_vertical;
		}
		return m_k == line.m_k;
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
		// check_intersections();
		return m_allIntersections.size();
	}

private:
	vector<Line> m_allLines;
	vector<Circle> m_allCircles;
	unordered_set<Node> m_allIntersections;

	void Input(string in_file)
	{
		char geoType;
		int N;

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
				Line curLine(x1, y1, x2, y2);
				m_allLines.push_back(curLine);
			}
			else if (geoType == 'C')
			{
				int x, y, r;
				ifile >> x >> y >> r;
				Circle curCircle(x, y, r);
				m_allCircles.push_back(curCircle);

			}
		}
		ifile.close();
		return;
	}

	void IntersectLLC()
	{
		if (m_allLines.size() == 0)
		{
			return;
		}
		for (vector<Line>::iterator iterOut = m_allLines.begin(); iterOut != m_allLines.end(); iterOut++)
		{
			Line line1 = *iterOut;
			for (vector<Line>::iterator iterIn = iterOut + 1; iterIn != m_allLines.end(); iterIn++)
			{
				Line line2 = *iterIn;
				vector<Node> intersections = line1.IntersectLine(line2);
				for (vector<Node>::iterator iterInter = intersections.begin(); iterInter != intersections.end(); iterInter++)
				{
					m_allIntersections.insert(*iterInter);
				}
			}
			for (vector<Circle>::iterator iter = m_allCircles.begin(); iter != m_allCircles.end(); iter++)
			{
				Circle circle = *iter;
				vector<Node> intersections = line1.IntersectCircle(circle);
				for (vector<Node>::iterator iterInter = intersections.begin(); iterInter != intersections.end(); iterInter++)
				{
					m_allIntersections.insert(*iterInter);
				}
			}
		}
		return;
	}

	void IntersectCC()
	{
		if (m_allCircles.size() == 0)
		{
			return;
		}
		for (vector<Circle>::iterator iterOut = m_allCircles.begin(); iterOut != m_allCircles.end(); iterOut++)
		{
			for (vector<Circle>::iterator iterIn = iterOut + 1; iterIn != m_allCircles.end(); iterIn++)
			{
				Circle circle1 = *iterOut, circle2 = *iterIn;
				vector<Node> intersections = circle1.Intersect(circle2);
				for (vector<Node>::iterator iterInter = intersections.begin(); iterInter != intersections.end(); iterInter++)
				{
					m_allIntersections.insert(*iterInter);
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
		for (unordered_set<Node>::iterator iter = m_allIntersections.begin(); iter != m_allIntersections.end(); iter++)
		{
			Node intersection = *iter;
			cout << intersection.getX() << ' ' << intersection.getY() << endl;
		}
		return;
	}
};
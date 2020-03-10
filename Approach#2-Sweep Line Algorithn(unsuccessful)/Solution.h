#pragma once
#include "Line.h"
#include "Node.h"


class SweepLine
{
public:
	void Solve()
	{
		Input();
		DealParallel();
		BuildDataStructures();
		Sweep();
		Output();
	}

private:
	vector<Line> m_parallelLines;
	set<Intercept> m_intercepts;
	InterceptLinkedList m_sweepLinkedList;
	queue<Intersection> m_sweepQueue;
	unordered_set<Node> m_queuedIntersections;
	unordered_set<Node> m_intersections;

	void Input()
	{
		int N;
		cin >> N;
		while (N--)
		{
			char geoType;
			int x1, y1, x2, y2;
			
			cin >> geoType >> x1 >> y1 >> x2 >> y2;
			Line newLine = Line(x1, y1, x2, y2);
			if (newLine.m_parallel)
			{
				m_parallelLines.push_back(newLine);
			}
			else
			{
				Intercept newIntercept = newLine.InterceptX();
				m_intercepts.insert(newIntercept);
			}
		}
		return;
	}

	void DealParallel()
	{
		for (vector<Line>::iterator iterLine = m_parallelLines.begin(); iterLine != m_parallelLines.end(); iterLine++)
		{
			for (set<Intercept>::iterator iterIntercept = m_intercepts.begin(); iterIntercept != m_intercepts.end(); iterIntercept++)
			{
				vector<Node> intersections = iterLine->Intersect(*iterIntercept->m_line);
				for (vector<Node>::iterator iterNode = intersections.begin(); iterNode != intersections.end(); iterNode++)
				{
					m_intersections.insert(*iterNode);
				}
			}
		}
		return;
	}

	void BuildDataStructures()
	{
		Intercept* curIntercept;

		for (set<Intercept>::iterator iterIntercept = m_intercepts.begin(); iterIntercept != m_intercepts.end(); iterIntercept++)
		{
			curIntercept = new Intercept(*iterIntercept);
			m_sweepLinkedList.Insert(curIntercept);
			if (!m_sweepLinkedList.atHead(curIntercept))
			{
				vector<Intersection> intersections = curIntercept->m_previous->Intersect(*curIntercept);
				int monitor = intersections.size();
				if (intersections.size() == 1)
				{
					Intersection intersection = intersections[0];
					if (m_queuedIntersections.find(intersection) == m_queuedIntersections.end())
					{
						m_queuedIntersections.insert(intersection);
						m_intersections.insert(intersection);
						m_sweepQueue.push(intersection);
					}
				}
			}
		}
		return;
	}

	void Sweep()
	{
		while (!m_sweepQueue.empty())
		{
			Intersection curIntersection = m_sweepQueue.front();
			Intercept* left = curIntersection.m_leftIntercept;
			Intercept* right = curIntersection.m_rightIntercept;
			m_sweepLinkedList.Swap(left, right);
			if (!m_sweepLinkedList.atHead(right))
			{
				vector<Intersection> intersections = right->m_previous->Intersect(*right);
				if (intersections.size() == 1)
				{
					Intersection intersection = intersections[0];
					if (m_queuedIntersections.find(intersection) == m_queuedIntersections.end())
					{
						m_queuedIntersections.insert(intersection);
						m_intersections.insert(intersection);
						m_sweepQueue.push(intersection);
					}
				}
			}
			if (!m_sweepLinkedList.atTail(left))
			{
				vector<Intersection> intersections = left->Intersect(*left->m_next);
				if (intersections.size() == 1)
				{
					Intersection intersection = intersections[0];
					if (m_queuedIntersections.find(intersection) == m_queuedIntersections.end())
					{
						m_queuedIntersections.insert(intersection);
						m_intersections.insert(intersection);
						m_sweepQueue.push(intersection);
					}
				}
			}
			m_sweepQueue.pop();
		}
		return;
	}

	void Output()
	{
		cout << m_intersections.size();
		return;
	}
};
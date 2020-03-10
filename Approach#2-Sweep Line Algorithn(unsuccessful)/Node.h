#pragma once
#include "global.h"


// Declare node-related classes
class Line;
class Intersection;
class Node
{
public:
	double m_x, m_y;

	Node(double, double);

	bool operator==(const Node&) const;
};
namespace std
{
	template<>
	struct hash<Node>
	{
		size_t operator()(const Node& obj) const
		{
			return hash<long long>()(llround(obj.m_x)) + hash<long long>()(llround(obj.m_y));
		}
	};

}


class Intercept : public Node
{
public:
	Line* m_line;
	Intercept* m_next;
	Intercept* m_previous;

	Intercept(double, double, Line*);

	bool operator<(const Intercept&) const;

	vector<Intersection> Intersect(Intercept);
};


class Intersection : public Node
{
public:
	Intersection(Node, Intercept*, Intercept*);

	Intercept* m_leftIntercept;
	Intercept* m_rightIntercept;
};


class InterceptLinkedList
{
public:
	InterceptLinkedList();

	bool atTail(Intercept* intercept);

	bool atHead(Intercept* intercept);

	void Insert(Intercept*);

	void Swap(Intercept*, Intercept*);

private:
	Intercept* m_head;
	Intercept* m_tail;
};

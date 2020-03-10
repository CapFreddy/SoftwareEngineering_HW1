#include "Line.h"
#include "Node.h"


// Define node-related classes
Node::Node(double x, double y) : m_x(x), m_y(y) {}

bool Node::operator==(const Node& obj) const { return abs(m_x - obj.m_x) < 1e-6 && abs(m_y - obj.m_y) < 1e-6; }


Intercept::Intercept(double x, double y, Line* line) : Node(x, y), m_line(line) {}

bool Intercept::operator<(const Intercept& obj) const { return m_x <= obj.m_x; }

vector<Intersection> Intercept::Intersect(Intercept intercept)
{
	vector<Intersection> intersections;
	vector<Node> nodes = m_line->Intersect(*intercept.m_line);
	if (nodes.size() == 1)
	{
		Node node = nodes[0];
		intersections.push_back(Intersection(node, this, &intercept));
	}

	return intersections;
}


Intersection::Intersection(Node node, Intercept* leftIntercept, Intercept* rightIntercept) : 
	Node(node), m_leftIntercept(leftIntercept), m_rightIntercept(rightIntercept) {}


InterceptLinkedList::InterceptLinkedList() : m_head(new Intercept(0, 0, nullptr)), m_tail(m_head) { m_tail->m_next = nullptr; }

bool InterceptLinkedList::atTail(Intercept* intercept) { return intercept == m_tail; }

bool InterceptLinkedList::atHead(Intercept* intercept) { return intercept == m_head->m_next; }

void InterceptLinkedList::Insert(Intercept* intercept)
{
	m_tail->m_next = intercept;
	m_tail->m_next->m_previous = m_tail;
	m_tail = m_tail->m_next;
	m_tail->m_next = nullptr;
	return;
}

void InterceptLinkedList::Swap(Intercept* left, Intercept* right)
{
	Intercept* temp;
	if (right == m_tail)
	{
		if (left->m_next == right)
		{
			right->m_next = left;
			right->m_previous = left->m_previous;
			left->m_next = nullptr;
			left->m_previous->m_next = right;
			left->m_previous = right;
		}
		else
		{
			right->m_next = left->m_next;
			right->m_next = left;
			temp = right->m_previous;
			right->m_previous = left->m_previous;
			left->m_next->m_previous = right;
			left->m_next = nullptr;
			left->m_previous->m_next = right;
			left->m_previous = temp;
		}
		m_tail = left;
	}
	else
	{
		if (left->m_next == right)
		{
			right->m_next->m_previous = left;
			temp = right->m_next;
			right->m_next = left;
			right->m_previous = left->m_previous;
			left->m_next = temp;
			left->m_previous->m_next = right;
			left->m_previous = right;
		}
		else
		{
			right->m_next->m_previous = left;
			temp = right->m_next;
			right->m_next = left->m_next;
			left->m_next->m_previous = right;
			left->m_next = temp;
			right->m_previous->m_next = left;
			temp = right->m_previous;
			right->m_previous = left->m_previous;
			left->m_previous->m_next = right;
			left->m_previous = temp;
		}
	}
	return;
}

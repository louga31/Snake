#include "stdafx.h"

#include "HNode.h"

using namespace sf;

HNode::HNode(const unsigned x, const unsigned y) : m_x(x), m_y(y)
{

}

HNode::HNode(const HNode& node) : m_x(node.m_x), m_y(node.m_y)
{
	std::cout << "Copied HNode x:" << m_x << " y:" << m_y << std::endl;
}

bool operator==(const HNode& left, const HNode& right)
{
	return left.m_x == right.m_x && left.m_y == right.m_y;
}

bool operator!=(const HNode& left, const HNode& right)
{
	return left.m_x != right.m_x || left.m_y != right.m_y;
}

void HNode::ResetForAStar()
{
	m_alreadyVisited = false;
	m_shortestDistanceToThisPoint = 0;
}

HDirection HNode::GetDirectionTo(const HNode& other) const
{
	return HDirection{ static_cast<int>(other.m_x - m_x), static_cast<int>(other.m_y - m_y) };
}
#include "HNode.h"


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "HDirection.h"
#include "HEdge.h"
#include "Utils.h"

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

void HNode::SetEdges(std::vector<HNode>& allNodes)
{
	for (auto& node : allNodes)
	{
		if (dist(node.m_x, node.m_y, m_x, m_y) == 1)
		{
			m_edges.push_back(&node);
		}
	}
}

void HNode::SetSpanningTreeEdges(const std::vector<HEdge>& spanningTree)
{
	for (const auto& edge : spanningTree)
	{
		if (edge.Contains(*this))
		{
			m_spanningTreeAdjacentNodes.push_back(&edge.GetOtherNode(*this));
		}
	}
}

HDirection HNode::GetDirectionTo(const HNode& other) const
{
	return HDirection{ static_cast<int>(other.m_x - m_x), static_cast<int>(other.m_y - m_y) };
}

HNode HNode::GetNextNodeMovingLeft(const HNode& previousNode)
{
	const auto direction = previousNode.GetDirectionTo(*this);

	std::vector<HDirection> possibleDirections;
	for (auto& node : m_spanningTreeAdjacentNodes)
	{
		possibleDirections.push_back(GetDirectionTo(*node));
	}

	auto checkingDirection = GetLeftOf(direction);
	
	while (!FindInVector(possibleDirections, checkingDirection).first)
	{
		checkingDirection = getRightOf(checkingDirection);
	}
	return *m_spanningTreeAdjacentNodes[FindInVector(possibleDirections, checkingDirection).second];
}

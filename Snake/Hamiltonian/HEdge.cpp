#include "stdafx.h"

#include "HEdge.h"

#include "HNode.h"


HEdge::HEdge(HNode& node1, HNode& node2) : m_node1(node1), m_node2(node2)
{

}

HEdge::HEdge(const HEdge& edge) : m_node1(edge.m_node1), m_node2(edge.m_node2)
{
	std::cout << "Copied HEdge x1:" << m_node1.m_x << " y1:" << m_node1.m_y << " x2:" << m_node2.m_x << " y2:" << m_node2.m_y << std::endl;
}

bool HEdge::IsEqualTo(const HEdge& otherEdge) const
{
	return m_node1 == otherEdge.m_node1 && m_node2 == otherEdge.m_node2 || m_node1 == otherEdge.m_node2 && m_node2 == otherEdge.m_node1;
}

bool HEdge::Contains(const HNode& node) const
{
	return node == m_node1 || node == m_node2;
}

HNode& HEdge::GetOtherNode(const HNode& node) const
{
	if (node == m_node1)
	{
		return m_node2;
	}
	return m_node1;
}

void HEdge::ConnectNodes() const
{
	m_node1.m_spanningTreeAdjacentNodes.push_back(&m_node2);
	m_node2.m_spanningTreeAdjacentNodes.push_back(&m_node1);
}

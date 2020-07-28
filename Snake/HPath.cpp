#include "HPath.h"

#include <memory>

#include "HNode.h"
#include "Utils.h"

HPath::HPath(HNode& startingNode, HNode& finishingNode) : m_startingNode(startingNode), m_finishingNode(finishingNode)
{
	
}

HPath HPath::Clone()
{
	HPath clone(*m_nodesInPath[0], m_finishingNode);
	clone.m_nodesInPath.insert(clone.m_nodesInPath.end(), m_nodesInPath.begin()+1, m_nodesInPath.end());
	clone.m_pathLength = m_pathLength;
	clone.m_distanceToApple = m_distanceToApple;

	return clone;
}

HNode HPath::GetLastNode()
{
	return *m_nodesInPath[m_nodesInPath.size() - 1];
}

void HPath::SetDistanceToApple()
{
	
	m_distanceToApple = dist(m_finishingNode.m_x, m_finishingNode.m_y, GetLastNode().m_x, GetLastNode().m_y);
}

void HPath::AddToTail(HNode& node)
{
	m_nodesInPath.push_back(&node);
	m_pathLength += 1;
	SetDistanceToApple();
}

HNode HPath::GetSnakeTailPositionAfterFollowingPath(unsigned& snakeLength)
{
	return *m_nodesInPath[m_pathLength - snakeLength]; // Not sure (432)
}

std::pair<unsigned, unsigned> HPath::GetNextMove()
{
	auto x = m_nodesInPath[m_pathCounter + 1]->m_x - m_nodesInPath[m_pathCounter]->m_x;
	auto y = m_nodesInPath[m_pathCounter + 1]->m_y - m_nodesInPath[m_pathCounter]->m_y;
	m_pathCounter++;
	return { x, y };
}

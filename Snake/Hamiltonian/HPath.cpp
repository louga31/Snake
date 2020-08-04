#include "stdafx.h"

#include "HPath.h"

#include "HamiltonianCycle.h"

#include "Game/Snake.h"

HPath::HPath(HNode* startingNode, HNode* finishingNode) : m_startingNode(startingNode), m_finishingNode(finishingNode)
{
	m_nodesInPath.push_back(m_startingNode);
	SetDistanceToApple();
}

HNode& HPath::GetLastNode()
{
	return *m_nodesInPath[m_nodesInPath.size() - 1];
}

void HPath::SetDistanceToApple()
{
	m_distanceToApple = dist(m_finishingNode->m_x, m_finishingNode->m_y, GetLastNode().m_x, GetLastNode().m_y);
}

void HPath::AddToTail(HNode& node)
{
	m_nodesInPath.push_back(&node);
	m_pathLength += 1;
	SetDistanceToApple();
}

HNode& HPath::GetSnakeTailPositionAfterFollowingPath(Snake& snake, HamiltonianCycle& hamCycle, unsigned cellSize)
{
	if (static_cast<int>(m_pathLength) - 1 < snake.m_tailBlocks.size() || static_cast<int>(m_pathLength) - 1 < 0)
	{
		return *hamCycle.m_cycle[hamCycle.GetNodeNo(snake.m_tailBlocks[std::max(0, static_cast<int>(m_pathLength - 1))])];
	}
	
	return *m_nodesInPath[snake.m_tailBlocks.size()];
}

Vector2i HPath::GetNextMove()
{
	const int x = m_nodesInPath[m_pathCounter + 1]->m_x - m_nodesInPath[m_pathCounter]->m_x;
	const int y = m_nodesInPath[m_pathCounter + 1]->m_y - m_nodesInPath[m_pathCounter]->m_y;
	m_pathCounter++;
	return {x, y};
}

HPath HPath::Clone()
{
	HPath clone(m_nodesInPath[0], m_finishingNode);
	clone.m_nodesInPath.insert(clone.m_nodesInPath.end(), m_nodesInPath.begin() + 1, m_nodesInPath.end());
	clone.m_pathLength = m_pathLength;
	clone.m_distanceToApple = m_distanceToApple;

	return clone;
}

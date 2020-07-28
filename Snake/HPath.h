#pragma once
#include <memory>
#include <vector>

#include "HNode.h"

class HPath
{
public:
	HPath(HNode& startingNode, HNode& finishingNode);

	HPath Clone();

private:
	HNode& m_startingNode;
	HNode& m_finishingNode;
	
	unsigned m_pathLength = 0;
	std::vector<HNode*> m_nodesInPath{ &m_startingNode };

	unsigned m_distanceToApple = 0;
	unsigned m_pathCounter = 0;

	HNode GetLastNode();
	void SetDistanceToApple();
	void AddToTail(HNode& node);
	HNode GetSnakeTailPositionAfterFollowingPath(unsigned& snakeLength);
	std::pair<unsigned, unsigned > GetNextMove();
};

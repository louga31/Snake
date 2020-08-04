#pragma once

class Snake;
class HamiltonianCycle;

class HPath
{
public:
	HPath(HNode* startingNode, HNode* finishingNode);
	
	HNode* m_startingNode;
	HNode* m_finishingNode;
	
	unsigned m_pathLength = 0;
	std::vector<HNode*> m_nodesInPath;
	
	unsigned m_distanceToApple = 0;
	unsigned m_pathCounter = 0;

	HNode& GetLastNode();
	void SetDistanceToApple();
	void AddToTail(HNode& node);
	HNode& GetSnakeTailPositionAfterFollowingPath(Snake& snake, HamiltonianCycle& hamCycle, unsigned cellSize);
	Vector2i GetNextMove();

	HPath Clone();
};

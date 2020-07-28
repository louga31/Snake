#pragma once
#include "HEdge.h"

#include <SFML/Graphics.hpp>

using namespace sf;

class HamiltonianCycle
{
public:
	HamiltonianCycle(RenderWindow& window, unsigned width, unsigned height);

	std::pair<bool, HNode > GetNextPosition(unsigned x, unsigned y);
	int GetNodeNo(unsigned x, unsigned y);
	std::vector<unsigned> GetPossiblePositionsFrom(unsigned x, unsigned y);
	void Draw();

private:
	RenderWindow& m_window;
	unsigned m_width;
	unsigned m_height;

	RectangleShape m_nodeLine;

	std::vector<HNode> m_stNodes;

	std::vector<HEdge> m_spanningTree;
	std::vector<HNode*> m_spanningTreeNodes;
	std::vector<HNode*> m_cycle;

	void CreateSpanningTree();
	void CreateCycle();
};

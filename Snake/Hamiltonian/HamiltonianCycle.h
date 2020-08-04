#pragma once
#include "../stdafx.h"

#include "HEdge.h"
#include "HNode.h"

using namespace sf;

class HamiltonianCycle
{
public:
	HamiltonianCycle(RenderWindow& window, unsigned cellSize, size_t width, size_t height);
	
	std::vector<HNode> m_stNodes;

	std::vector<HEdge> m_spanningTree;
	std::vector<HNode*> m_cycle;

	void Draw() const;
	HNode* GetNextPosition(unsigned x, unsigned y);
	unsigned GetNodeNo(Vector2f position);
	unsigned GetNodeNo(unsigned x, unsigned y);
	std::vector<unsigned> GetPossiblePositionsFrom(unsigned x, unsigned y);
	
private:
	RenderWindow& m_window;
	unsigned m_cellSize;
	size_t m_width, m_height;

	std::vector<HNode> m_cycleNodes;

	// Draw variables
	Font m_font;
	std::vector<Text> m_nodeNumbers;
	std::vector<VertexArray> m_lines;
	
	void CreateSpanningTree();
	void CreateCycle();
};

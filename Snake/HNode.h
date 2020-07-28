﻿#pragma once
#include <vector>

class HEdge;

#include "HDirection.h"

class HNode
{
public:
	HNode(unsigned x, unsigned y);

	HNode(const HNode& node);

	unsigned m_x;
	unsigned m_y;
	int m_cycleNo = -1;

	std::vector<HNode*> m_spanningTreeAdjacentNodes;
	std::vector<HNode*> m_edges;

	void SetEdges(std::vector<HNode>& allNodes);
	void SetSpanningTreeEdges(const std::vector<HEdge>& spanningTree);
	HDirection GetDirectionTo(const HNode& other) const;
	HNode GetNextNodeMovingLeft(const HNode& previousNode);

	
private:

	//A* variables
	bool m_alreadyVisited = false;
	unsigned m_shortestDistanceToThisPoint = 0;
	void ResetForAStar();
	
};

////////////////////////////////////////////////////////////
/// \relates HNode
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two HNode.
///
/// \param left  Left operand (a HNode)
/// \param right Right operand (a HNode)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
bool operator ==(const HNode& left, const HNode& right);

////////////////////////////////////////////////////////////
/// \relates HNode
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two vectors.
///
/// \param left  Left operand (a HNode)
/// \param right Right operand (a HNode)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
bool operator !=(const HNode& left, const HNode& right);
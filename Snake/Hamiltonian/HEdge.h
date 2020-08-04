#pragma once

#include "HNode.h"

class HEdge
{
public:
	HEdge(HNode& node1, HNode& node2);

	HEdge(const HEdge& edge);

	bool IsEqualTo(const HEdge& otherEdge) const;

	bool Contains(const HNode& node) const;

	HNode& GetOtherNode(const HNode& node) const;

	void ConnectNodes() const;

private:
	HNode& m_node1;
	HNode& m_node2;

};
#pragma once

#include "HNode.h"

class HEdge
{
public:
	HEdge(HNode& node1, HNode& node2);

	HEdge(const HEdge& edge);

	[[nodiscard]] bool IsEqualTo(const HEdge& otherEdge) const;

	[[nodiscard]] bool Contains(const HNode& node) const;

	[[nodiscard]] HNode& GetOtherNode(const HNode& node) const;

	void ConnectNodes() const;

private:
	HNode& m_node1;
	HNode& m_node2;

};

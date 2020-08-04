#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include "../Snake/Utils.h"
#include "../Snake/Hamiltonian/HNode.h"


boost::test_tools::predicate_result ValidateEdges(std::vector<HNode>& allNodes, HNode& node)
{
	for (auto& n : allNodes)
	{
		if (dist(n.m_x, n.m_y, node.m_x, node.m_y) == 1 && !FindInVector(node.m_edges, &n).first)
		{
			return false;
		}
	}

	for (auto edge : node.m_edges)
	{	
		if (dist(edge->m_x, edge->m_y, node.m_x, node.m_y) != 1)
		{
			return false;
		}
	}

	return true;
}

BOOST_AUTO_TEST_SUITE(HNode_SetEdges_Test)

	BOOST_AUTO_TEST_CASE(SetEdgesSquareSmall)
	{
		const auto width = 30, height = 30;
		std::vector<HNode> allNodes;
		allNodes.reserve(width * height);
		for (auto i = 0; i < width / 2; i++)
		{
			for (auto j = 0; j < height / 2; j++)
			{
				allNodes.emplace_back(i, j);
			}
		}
	
		for (auto& node : allNodes)
		{
			node.SetEdges(allNodes);
		}

		for (auto& node : allNodes)
		{
			BOOST_CHECK(ValidateEdges(allNodes, node));
		}
	}

	BOOST_AUTO_TEST_CASE(SetEdgesSquareBig)
	{
		const auto width = 200, height = 100;
		std::vector<HNode> allNodes;
		allNodes.reserve(width * height);
		for (auto i = 0; i < width / 2; i++)
		{
			for (auto j = 0; j < height / 2; j++)
			{
				allNodes.emplace_back(i, j);
			}
		}

		for (auto& node : allNodes)
		{
			node.SetEdges(allNodes);
		}

		for (auto& node : allNodes)
		{
			BOOST_CHECK(ValidateEdges(allNodes, node));
		}
	}

	BOOST_AUTO_TEST_CASE(SetEdgesRectangleSmall)
	{
		const auto width = 50, height = 30;
		std::vector<HNode> allNodes;
		allNodes.reserve(width * height);
		for (auto i = 0; i < width / 2; i++)
		{
			for (auto j = 0; j < height / 2; j++)
			{
				allNodes.emplace_back(i, j);
			}
		}

		for (auto& node : allNodes)
		{
			node.SetEdges(allNodes);
		}

		for (auto& node : allNodes)
		{
			BOOST_CHECK(ValidateEdges(allNodes, node));
		}
	}

	BOOST_AUTO_TEST_CASE(SetEdgesRectangleBig)
	{
		const auto width = 200, height = 100;
		std::vector<HNode> allNodes;
		allNodes.reserve(width * height);
		for (auto i = 0; i < width / 2; i++)
		{
			for (auto j = 0; j < height / 2; j++)
			{
				allNodes.emplace_back(i, j);
			}
		}

		for (auto& node : allNodes)
		{
			node.SetEdges(allNodes);
		}

		for (auto& node : allNodes)
		{
			BOOST_CHECK(ValidateEdges(allNodes, node));
		}
	}

BOOST_AUTO_TEST_SUITE_END()
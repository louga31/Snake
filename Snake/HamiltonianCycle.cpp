#include "HamiltonianCycle.h"


#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <SFML/Graphics.hpp>

#include "HEdge.h"
#include "HNode.h"
#include "Utils.h"

using namespace sf;

HamiltonianCycle::HamiltonianCycle(RenderWindow& window, const unsigned width, const unsigned height) : m_window(window), m_width(width), m_height(height)
{
	try
	{
		CreateCycle();
	} catch(std::out_of_range const& e)
	{
		std::cerr << "ERROR : " << e.what() << std::endl;
		m_window.close();
	}
		
	
}

std::pair<bool, HNode > HamiltonianCycle::GetNextPosition(const unsigned x, const unsigned y)
{
	for (auto i = 0; i < m_cycle.size(); i++)
	{
		if (m_cycle[i]->m_x == x && m_cycle[i]->m_y == y)
		{
			return { true, *m_cycle[(i + 1) % m_cycle.size()] };
		}
	}
	return { false, *m_cycle[0] };
}

int HamiltonianCycle::GetNodeNo(const unsigned x, const unsigned y)
{
	for (auto i = 0; i < m_cycle.size(); i++)
	{
		if (m_cycle[i]->m_x == x && m_cycle[i]->m_y == y)
		{
			return i;
		}
	}
	return -1;
}

std::vector<unsigned> HamiltonianCycle::GetPossiblePositionsFrom(unsigned x, unsigned y)
{
	auto* currentNode = m_cycle[GetNodeNo(x, y)];
	std::vector<unsigned> nodeNos;
	for (auto& node : currentNode->m_edges)
	{
		nodeNos.push_back(GetNodeNo(node->m_x, node->m_y));
	}
	return nodeNos;
}

void HamiltonianCycle::Draw()
{
	m_nodeLine.setSize(Vector2f(4, 16));
	m_nodeLine.setFillColor(Color::Blue);
	m_nodeLine.setPosition(0,0);

	m_window.draw(m_nodeLine);
}

void HamiltonianCycle::CreateSpanningTree()
{
	m_stNodes.reserve((m_width / 2 + 1) * (m_height / 2 + 1));
	for (unsigned i = 0; i <= m_width / 2; i++)
	{
		for (unsigned j = 0; j <= m_height / 2; j++)
		{
			m_stNodes.emplace_back(i, j);
		}
	}

	for (auto& node : m_stNodes)
	{
		node.SetEdges(m_stNodes);
	}

	m_spanningTree.reserve(m_width * m_height);
	auto& randomNode = m_stNodes[floor(rand() % m_stNodes.size())];
	m_spanningTree.emplace_back(randomNode, *randomNode.m_edges[0]);

	std::vector<HNode*> nodesInSpanningTree{ &randomNode, randomNode.m_edges[0] };

	while (nodesInSpanningTree.size() < m_stNodes.size())
	{
		randomNode = *RandomElement(nodesInSpanningTree);
		
		std::vector<HNode*> edges;
		for (auto* node : randomNode.m_edges)
		{
			if (!FindInVector(nodesInSpanningTree, node).first) {
				edges.push_back(node);
			}
		}
		
		if (!edges.empty())
		{
			auto* randomEdge = RandomElement(edges);
			nodesInSpanningTree.push_back(randomEdge);
			m_spanningTree.emplace_back(randomNode, *randomEdge);
		}
	}

	for (auto& node : m_stNodes)
	{
		node.SetSpanningTreeEdges(m_spanningTree);
	}

	// spanning tree created

	for (auto& node : m_stNodes)
	{
		
		if (!FindInVector(nodesInSpanningTree, &node).first) {
			throw std::out_of_range("An error happened when creating spanning tree");
		}
	}

	for (auto& node : m_stNodes)
	{
		m_spanningTreeNodes.push_back(&node);
	}
	
}

void HamiltonianCycle::CreateCycle()
{
	CreateSpanningTree();

	std::vector<HNode> cycleNodes;
	cycleNodes.reserve(m_width * m_height);
	for (unsigned i = 0; i < m_width; i++)
	{
		for (unsigned j = 0; j < m_height; j++)
		{
			cycleNodes.emplace_back(i, j);
		}
	}

	for (auto& node : cycleNodes)
	{
		node.SetEdges(cycleNodes);
	}

	for (auto* currentSpanningTreeNode : m_spanningTreeNodes)
	{
		for (auto* other : currentSpanningTreeNode->m_spanningTreeAdjacentNodes)
		{
			auto connectNodes = [this, &cycleNodes](const unsigned x1, const unsigned y1, const unsigned x2, const unsigned y2)
			{
				if (y1 + m_height * x1 >= cycleNodes.size() || y2 + m_height * x2 >= cycleNodes.size())
				{
					return;
				}
				auto& a = cycleNodes[y1 + m_height * x1];
				auto& b = cycleNodes[y2 + m_height * x2];

				a.m_spanningTreeAdjacentNodes.push_back(&b);
				b.m_spanningTreeAdjacentNodes.push_back(&a);
			};

			const auto direction = currentSpanningTreeNode->GetDirectionTo(*other);
			const auto x = currentSpanningTreeNode->m_x * 2;
			const auto y = currentSpanningTreeNode->m_y * 2;

			if (direction.x == 1)
			{
				//is to the right
				//CONNECT THEM
				/*
				 *    *----*    *
					a         b
				 *    *----*    *

				 */
				connectNodes(x + 1, y, x + 2, y);
				connectNodes(x + 1, y + 1, x + 2, y + 1);
			}
			else if (direction.y == 1) {
				connectNodes(x, y + 1, x, y + 2);
				connectNodes(x + 1, y + 1, x + 1, y + 2);
			}
		}
	}

	//make a list of all the nodes which only have 1 adjacent node
	//then make a list of all the edges we need to add
	std::vector<HNode*> degree1Nodes;
	for (auto& node : cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() == 1)
		{
			degree1Nodes.push_back(&node);
		}
	}

	std::vector<std::shared_ptr<HEdge>> newEdges;
	for (auto* node : degree1Nodes)
	{
		//get the direction from the other node to this one
		auto d = node->m_spanningTreeAdjacentNodes[0]->GetDirectionTo(*node);
		//add that direction again to get the next node
		d.x += static_cast<int>(node->m_x);
		d.y += static_cast<int>(node->m_y);
		//d now points to the new node
		std::shared_ptr<HEdge> newEdge = std::make_unique<HEdge>(cycleNodes[d.y + m_height * d.x], *node);
		auto uniqueEdge = true;
		for (const auto& edge : newEdges)
		{
			if (edge->IsEqualTo(*newEdge))
			{
				uniqueEdge = false;
				break;
			}
		}

		if (uniqueEdge)
		{
			newEdges.push_back(newEdge);
		}
	}

	for (const auto& edge : newEdges)
	{
		edge->ConnectNodes();
	}

	//do it again to get the end nodes
	degree1Nodes.clear();
	for (auto& node : cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() == 1)
		{
			degree1Nodes.push_back(&node);
		}
	}

	newEdges.clear();
	for (auto* node : degree1Nodes)
	{
		HDirection d{ node->m_x, node->m_y };
		for (auto* node2 : degree1Nodes)
		{
			if (dist(node->m_x, node->m_y, node2->m_x, node2->m_y) == 1)
			{
				if (floor(node->m_x / 2) == floor(node2->m_x / 2) && floor(node->m_y / 2) == floor(node2->m_y / 2))
				{
					std::shared_ptr<HEdge> newEdge = std::make_shared<HEdge>(*node2, *node);
					auto uniqueEdge = true;
					for (const auto& edge : newEdges)
					{
						if (edge->IsEqualTo(*newEdge))
						{
							uniqueEdge = false;
							break;
						}
					}
					if (uniqueEdge)
					{
						newEdges.push_back(newEdge);
					}

					break;
				}
			}
		}
	}

	for (const auto& edge : newEdges)
	{
		edge->ConnectNodes();
	}

	for (auto& node : cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() != 2)
		{
			throw std::out_of_range("An error happened when creating cycle");
		}
	}

	std::vector<HNode*> cycle {&RandomElement(cycleNodes)};

	auto* previous = cycle[0];
	auto* node = cycle[0]->m_spanningTreeAdjacentNodes[0];
	while (node != cycle[0])
	{
		auto* next = node->m_spanningTreeAdjacentNodes[0];
		if (next == previous)
		{
			next = node->m_spanningTreeAdjacentNodes[1];
		}
		
		if (next->m_spanningTreeAdjacentNodes.size() != 2)
		{
			//throw "An error happened when creating cycle";
		}
		cycle.push_back(node);
		previous = node;
		node = next;
	}

	m_cycle = cycle;
	for (unsigned i = 0; i < m_cycle.size() - 1; i++)
	{
		m_cycle[i]->m_cycleNo = i;
	}
}
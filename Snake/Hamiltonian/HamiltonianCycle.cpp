#include "stdafx.h"
#include "HamiltonianCycle.h"

#include "HNode.h"

#define FONT_SIZE 12

HamiltonianCycle::HamiltonianCycle(RenderWindow& window, unsigned cellSize, const size_t width, const size_t height) :m_window(window), m_cellSize(cellSize), m_width(width), m_height(height)
{
	m_stNodes.reserve(width / 2 * (height / 2));
	m_spanningTree.reserve(m_width * m_height);
	if (!m_font.loadFromFile("Fonts/arial.ttf"))
		throw "Could not load font";
	for (auto nodeNumber : m_nodeNumbers)
	{
		nodeNumber.setFont(m_font);
		const_cast<Texture&>(m_font.getTexture(FONT_SIZE)).setSmooth(true);
	}
	
	CreateCycle();
}

void HamiltonianCycle::Draw() const
{
	for (auto i = 0; i < m_cycle.size(); i++)
	{
		m_window.draw(m_nodeNumbers[i]);
		m_window.draw(m_lines[i]);
	}
}

HNode* HamiltonianCycle::GetNextPosition(unsigned x, unsigned y)
{
	for (auto i = 0; i < m_cycle.size(); i++)
	{
		if (m_cycle[i]->m_x == x && m_cycle[i]->m_y == y)
		{
			return m_cycle[(i + 1) % m_cycle.size()];
		}
	}
	return nullptr;
}

void HamiltonianCycle::CreateSpanningTree()
{
	for (unsigned i = 0; i < m_width / 2; i++)
	{
		for (unsigned j = 0; j < m_height / 2; j++)
		{
			m_stNodes.emplace_back(i, j);
		}
	}
	
	for (auto& node : m_stNodes)
	{
		node.SetEdges(m_stNodes);
	}

	auto* randomNode = &RandomElement(m_stNodes);
	m_spanningTree.emplace_back(*randomNode, *randomNode->m_edges[0]);
	std::vector<HNode*> nodesInSpanningTree{ randomNode, randomNode->m_edges[0] };

	while (nodesInSpanningTree.size() < m_stNodes.size())
	{
		randomNode = RandomElement(nodesInSpanningTree);
		auto edges = filter(randomNode->m_edges, [&nodesInSpanningTree](HNode* n) {return !FindInVector(nodesInSpanningTree, n).first; });
		if (!edges.empty())
		{
			auto* randomEdge = RandomElement(edges);
			nodesInSpanningTree.push_back(randomEdge);
			m_spanningTree.emplace_back(*randomNode, *randomEdge);
		}
	}

	for (auto& node : m_stNodes)
	{
		node.SetSpanningTreeEdges(m_spanningTree);
	}

	for (auto& node : m_stNodes)
	{
		if (!FindInVector(nodesInSpanningTree, &node).first)
		{
			std::cout << "Spanning tree creation error (node: " << node << ")" << std::endl;
		}
	}
}

void HamiltonianCycle::CreateCycle()
{
	CreateSpanningTree();

	m_cycleNodes.reserve(m_width * m_height);
	for (unsigned i = 0; i < m_width; i++)
	{
		for (unsigned j = 0; j < m_height; j++)
		{
			m_cycleNodes.emplace_back(i, j);
		}
	}

	for (auto& node : m_cycleNodes)
	{
		node.SetEdges(m_cycleNodes);
	}

	for (auto& currentSpanningTreeNode : m_stNodes)
	{
		for (auto* other : currentSpanningTreeNode.m_spanningTreeAdjacentNodes)
		{
			auto connectNodes = [this](int x1, int y1, int x2, int y2)
			{
				if (y1 + m_height * x1 >= m_cycleNodes.size() || y2 + m_height * x2 >= m_cycleNodes.size()) {
					return;
				}
				auto* a = &m_cycleNodes[y1 + m_height * x1];
				auto* b = &m_cycleNodes[y2 + m_height * x2];
				a->m_spanningTreeAdjacentNodes.push_back(b);
				b->m_spanningTreeAdjacentNodes.push_back(a);
			};

			const auto direction = currentSpanningTreeNode.GetDirectionTo(*other);
			const auto x = currentSpanningTreeNode.m_x * 2;
			const auto y = currentSpanningTreeNode.m_y * 2;
			
			if (direction.x == 1)
			{
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
	for (auto& node : m_cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() == 1)
			degree1Nodes.push_back(&node);
	}
	
	std::vector<std::shared_ptr<HEdge>> newEdges;
	for (auto* node : degree1Nodes)
	{
		//get the direction from the other node to this one
		auto d = node->m_spanningTreeAdjacentNodes[0]->GetDirectionTo(*node);
		//add that direction again to get the next node
		d.x += node->m_x;
		d.y += node->m_y;
		//d now points to the new node
		auto newEdge = std::make_shared<HEdge>(m_cycleNodes[d.y + m_height * d.x], *node);
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

	for (auto& edge : newEdges)
	{
		edge->ConnectNodes();
	}
	
	//do it again to get the end nodes
	degree1Nodes.clear();
	newEdges.clear();
	for (auto& node : m_cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() == 1)
			degree1Nodes.push_back(&node);
	}
	for (auto* n : degree1Nodes)
	{
		HDirection d{ n->m_x, n->m_y };
		for (auto* m : degree1Nodes)
		{
			if (dist(n->m_x, n->m_y, m->m_x, m->m_y) == 1)
			{
				if (floor(n->m_x / 2) == floor(m->m_x / 2) && floor(n->m_y / 2) == floor(m->m_y / 2))
				{
					auto newEdge = std::make_shared<HEdge>(*m, *n);
					auto uniqueEdge = true;
					for (auto& edge : newEdges)
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

	for (auto& edge : newEdges)
	{
		edge->ConnectNodes();
	}

	for (auto& node : m_cycleNodes)
	{
		if (node.m_spanningTreeAdjacentNodes.size() != 2)
		{
			std::cout << "CycleNodes creation error (node: " << node << ")" << std::endl;
		}
	}

	m_cycle.push_back(&RandomElement(m_cycleNodes));
	auto* previous = m_cycle[0];
	auto* node = m_cycle[0]->m_spanningTreeAdjacentNodes[0];
	
	while (node != m_cycle[0])
	{
		auto* next = node->m_spanningTreeAdjacentNodes[0];
		if (*next == *previous)
		{
			next = node->m_spanningTreeAdjacentNodes[1];
		}

		if (next->m_spanningTreeAdjacentNodes.size() != 2)
		{
			std::cout << "Cycle creation error (node: " << node << ")" << std::endl;
		}
		m_cycle.push_back(node);
		previous = node;
		node = next;
	}

	for (auto i = 0; i < m_cycle.size(); i++)
	{
		m_cycle[i]->m_cycleNo = i;
	}

	m_nodeNumbers.reserve(m_cycle.size());
	m_lines.reserve(m_cycle.size());
	for (auto i = 0; i < m_cycle.size(); i++)
	{
		m_nodeNumbers.emplace_back(std::to_string(i), m_font, FONT_SIZE);
		m_lines.emplace_back(Lines, 2);

		// Set Color
		m_nodeNumbers[i].setFillColor(Color::White);
		m_lines[i][0].color = Color(150, 150, 150, 150);
		m_lines[i][1].color = Color(150, 150, 150, 150);
		
		// Center numbers Origin
		const auto textRect = m_nodeNumbers[i].getLocalBounds();
		m_nodeNumbers[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		
		// Set Position
		Vector2f position(m_cellSize * m_cycle[i]->m_x + m_cellSize / 2, m_cellSize * m_cycle[i]->m_y + m_cellSize / 2);
		m_nodeNumbers[i].setPosition(position);
		m_lines[i][0].position = Vector2f(m_cycle[i]->m_x * m_cellSize + m_cellSize / 2, m_cycle[i]->m_y * m_cellSize + m_cellSize / 2);
		m_lines[i][1].position = Vector2f(m_cycle[(i + 1) % m_cycle.size()]->m_x * m_cellSize + m_cellSize / 2, m_cycle[(i + 1) % m_cycle.size()]->m_y * m_cellSize + m_cellSize / 2);
	}
}

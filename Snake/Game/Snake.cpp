#include "stdafx.h"

#include "Snake.h"

#include "Hamiltonian/HamiltonianCycle.h"

using namespace sf;

#define INITIAL_LENGTH 4

Snake::Snake(RenderWindow& window, HamiltonianCycle& hamCycle, const unsigned cellSize, FoodGenerator& foodGen) : m_window(window), m_hamCycle(hamCycle), m_cellSize(cellSize), m_foodGenerator(foodGen)
{
	AddCase();
	AddCase();
	AddCase();
	AddCase();
	m_foodGenerator.GenerateFruit(m_snakes);
}

void Snake::Move()
{
	// If enough time between move passed
	if (m_clock.getElapsedTime().asMicroseconds() >= m_timeToUpdate)
	{
		m_clock.restart();

		// Move all snake cells to previous one position, except for the first cell
		if (m_snakes.size() > 1)
		{
			for (int i = m_tailBlocks.size() - 1; i >= 0; i--)
			{
				if (i == 0)
				{
					m_tailBlocks[i] = Vector2f(m_snakes[0].getPosition().x / m_cellSize, static_cast<float>(m_snakes[0].getPosition().y / m_cellSize));
					m_snakes[i + 1].setPosition(m_tailBlocks[i] * static_cast<float>(m_cellSize));
					continue;
				}
				m_tailBlocks[i] = m_tailBlocks[i - 1];
				m_snakes[i + 1].setPosition(m_tailBlocks[i] * static_cast<float>(m_cellSize));
			}
		}
		
		if (m_isAI)
		{
			if (m_path == nullptr || m_path->m_pathCounter >= m_path->m_pathLength)
			{
				m_path = CalculatePath();
			}
			const auto nextMove = m_path->GetNextMove();
			m_snakes[0].move(static_cast<float>(nextMove.x * static_cast<int>(m_cellSize)), static_cast<float>(nextMove.y * static_cast<int>(m_cellSize)));
			
		}
		else
		{
			// Set first cell position based on movement direction
			m_snakes[0].move(static_cast<float>(m_direction.x * static_cast<int>(m_cellSize)), static_cast<float>(m_direction.y * static_cast<int>(m_cellSize)));
		}
		
		CheckCollision();
	}
}

void Snake::Update(Event& event)
{
	if (event.type == Event::KeyPressed)
	{
		if (event.key.code == Keyboard::PageUp)
		{
			m_timeToUpdate /= 2;
		}

		else if (event.key.code == Keyboard::PageDown)
		{
			m_timeToUpdate *= 2;
		}

		else if (event.key.code == Keyboard::I)
		{
			m_isAI = !m_isAI;
		}
		
		if (!m_isAI)
		{
			if (event.key.code == Keyboard::Left)
			{
				if (m_snakes[0].getPosition().x - m_snakes[0].getSize().x != m_snakes[1].getPosition().x)
				{
					if (m_direction != Direction::left && m_direction != Direction::right)
					{
						m_direction = Direction::left;
					}
				}
			}
			
			else if (event.key.code == Keyboard::Right)
			{
				if (m_snakes[0].getPosition().x + m_snakes[0].getSize().x != m_snakes[1].getPosition().x)
				{
					if (m_direction != Direction::right && m_direction != Direction::left)
					{
						m_direction = Direction::right;
					}
				}
			}

			else if (event.key.code == Keyboard::Up)
			{
				if (m_snakes[0].getPosition().y - m_snakes[0].getSize().x != m_snakes[1].getPosition().y)
				{
					if (m_direction != Direction::up && m_direction != Direction::down)
					{
						m_direction = Direction::up;
					}
				}
			}

			else if (event.key.code == Keyboard::Down)
			{
				if (m_snakes[0].getPosition().y + m_snakes[0].getSize().x != m_snakes[1].getPosition().y)
				{
					if (m_direction != Direction::down && m_direction != Direction::up)
					{
						m_direction = Direction::down;
					}
				}
			}
		}
	}
}

void Snake::Draw() const
{
	for (const auto& snakes : m_snakes)
		m_window.draw(snakes);
}

void Snake::CheckCollision()
{
	 // Own collision
	
	for (unsigned i = 2; i < m_snakes.size(); i++)
	{
		if (m_snakes[0].getPosition().x == m_snakes[i].getPosition().x
			&& m_snakes[0].getPosition().y == m_snakes[i].getPosition().y)
		{
			m_window.close();
		}
	}

	// Out of screen collisions
	
	if (m_snakes[0].getPosition().x < 0) // Snake got out on the left
	{
		m_snakes[0].setPosition(static_cast<float>(m_window.getSize().x - m_cellSize), m_snakes[0].getPosition().y);
	}

	if (m_snakes[0].getPosition().x + m_snakes[0].getSize().x > m_window.getSize().x) // Snake got out on the right
	{
		m_snakes[0].setPosition(0.0f, m_snakes[0].getPosition().y);
	}
	
	if (m_snakes[0].getPosition().y < 0) // Snake got out on the top
	{
		m_snakes[0].setPosition(m_snakes[0].getPosition().x, static_cast<float>(m_window.getSize().y - m_cellSize));
	}

	if (m_snakes[0].getPosition().y + m_snakes[0].getSize().y > m_window.getSize().y) // Snake got out on the bottom
	{
		m_snakes[0].setPosition(m_snakes[0].getPosition().x, 0.0f);
	}

	// Food Collision

	if (m_snakes[0].getPosition().x == m_foodGenerator.m_fruit.getPosition().x
		&& m_snakes[0].getPosition().y == m_foodGenerator.m_fruit.getPosition().y)
	{
		AddCase();
		m_foodGenerator.GenerateFruit(m_snakes);
		CalculatePath();
	}
}

void Snake::AddCase() // Add 1 more case
{
	RectangleShape cell;
	cell.setSize(Vector2f(static_cast<float>(m_cellSize), static_cast<float>(m_cellSize)));
	cell.setOutlineColor(Color::Black);
	cell.setOutlineThickness(1.0f);

	cell.setFillColor(Color::Green);
	
	if (m_snakes.size() < INITIAL_LENGTH)
	{
		// First cell is yellow and first INITIAL_LENGTH cells spawn on first nodes of cycle
		if (m_snakes.empty())
		{
			cell.setFillColor(Color::Yellow);
			cell.setPosition(m_hamCycle.m_cycle[INITIAL_LENGTH - 1]->m_x * m_cellSize, m_hamCycle.m_cycle[INITIAL_LENGTH - 1]->m_y * m_cellSize);

			// Set Direction based on cycle next node
			const auto direction = m_hamCycle.m_cycle[INITIAL_LENGTH - 1]->GetDirectionTo(*m_hamCycle.m_cycle[INITIAL_LENGTH]);
			if (direction.x == 0)
			{
				if (direction.y == 1)
				{
					m_direction = Direction::down;
				} else
				{
					m_direction = Direction::up;
				}
			} else
			{
				if (direction.x == 1)
				{
					m_direction = Direction::right;
				} else
				{
					m_direction = Direction::left;
				}
			}
		}
		else
		{
			m_tailBlocks.emplace_back(m_hamCycle.m_cycle[INITIAL_LENGTH - 1 - m_snakes.size()]->m_x, m_hamCycle.m_cycle[INITIAL_LENGTH - 1 - m_snakes.size()]->m_y);
			cell.setPosition(m_tailBlocks.back() * static_cast<float>(m_cellSize));
		}

	} else
	{
		if (m_tailBlocks.back().x == m_tailBlocks[m_tailBlocks.size() - 2].x)
		{
			if (m_tailBlocks.back().y - m_tailBlocks[m_tailBlocks.size() - 2].y > 0)
			{
				m_tailBlocks.emplace_back(m_tailBlocks.back().x, m_tailBlocks.back().y + 1);
				cell.setPosition(m_tailBlocks.back() * static_cast<float>(m_cellSize));
			}
			else
			{
				m_tailBlocks.emplace_back(m_tailBlocks.back().x, m_tailBlocks.back().y - 1);
				cell.setPosition(m_tailBlocks.back() * static_cast<float>(m_cellSize));
			}
		}
		else if (m_tailBlocks.back().y == m_tailBlocks[m_tailBlocks.size() - 2].y)
		{
			if (m_tailBlocks.back().x - m_tailBlocks[m_tailBlocks.size() - 2].x > 0)
			{
				m_tailBlocks.emplace_back(m_tailBlocks.back().x + 1, m_tailBlocks.back().y);
				cell.setPosition(m_tailBlocks.back() * static_cast<float>(m_cellSize));
			}
			else
			{
				m_tailBlocks.emplace_back(m_tailBlocks.back().x - 1, m_tailBlocks.back().y);
				cell.setPosition(m_tailBlocks.back() * static_cast<float>(m_cellSize));
			}
		} else
		{
			throw std::runtime_error("Failed to add snake cell");
		}
	}

	m_snakes.push_back(cell);
}

std::shared_ptr<HPath> Snake::CalculatePath()
{
	for (auto* node : m_hamCycle.m_cycle)
	{
		node->ResetForAStar();
	}
	
	m_appleCyclePosition = m_hamCycle.GetNodeNo(m_foodGenerator.m_fruit.getPosition().x / m_cellSize, m_foodGenerator.m_fruit.getPosition().y / m_cellSize);

	auto* startNode = m_hamCycle.m_cycle[m_hamCycle.GetNodeNo(m_snakes[0].getPosition().x / m_cellSize, m_snakes[0].getPosition().y / m_cellSize)];
	std::deque<std::shared_ptr<HPath>> bigList;

	std::shared_ptr<HPath> winningPath;

	bigList.emplace_back(std::make_shared<HPath>(startNode, m_hamCycle.m_cycle[m_appleCyclePosition]));

	while (true)
	{
		if (bigList.empty())
		{
			return winningPath;
		}
		auto currentPath = bigList[0];
		bigList.pop_front();
		if (winningPath != nullptr && currentPath->m_pathLength >= winningPath->m_pathLength)
		{
			continue;
		}

		if (currentPath->m_distanceToApple == 0) //path has found apple
		{
			if (winningPath == nullptr || currentPath->m_pathLength < winningPath->m_pathLength) {
				winningPath = std::make_shared<HPath>(*currentPath);
			}
			continue;
		}

		//if the final node has been visited and the previous visit was a shorter path then fuck this path
		auto& finalNodeInPath = currentPath->GetLastNode();

		if (!finalNodeInPath.m_alreadyVisited || currentPath->m_pathLength < finalNodeInPath.m_shortestDistanceToThisPoint)
		{
			//this is the shortest found path to this point
			finalNodeInPath.m_alreadyVisited = true;
			finalNodeInPath.m_shortestDistanceToThisPoint = currentPath->m_pathLength;

			//now we need to add all the paths possible from this node to the bigList
			for (auto* node : finalNodeInPath.m_edges)
			{
				if (OverTakesTail(node, &finalNodeInPath, &currentPath->GetSnakeTailPositionAfterFollowingPath(*this, m_hamCycle, m_cellSize))) // P
				{
					if (node->m_cycleNo != finalNodeInPath.m_cycleNo + 1)
					{
						continue;
					}
				}

				auto p = std::make_shared<HPath>(*currentPath);
				p->AddToTail(*node);
				if (p->GetLastNode().m_alreadyVisited && p->m_pathLength > p->GetLastNode().m_shortestDistanceToThisPoint)
				{
					continue;
				}
				bigList.push_back(p);
			}
		}

		//now we need to sort the bigList based on the distances to the apple plus the current distance of the path
		std::sort(bigList.begin(), bigList.end(), [](const std::shared_ptr<HPath>& a, const std::shared_ptr<HPath>& b) { return a->m_distanceToApple + a->m_pathLength < b->m_distanceToApple + b->m_pathLength;});
	}
}

bool Snake::OverTakesTail(HNode* newPos, HNode* h, HNode* t)
{
	const auto minDistanceBetweenHeadAndTail = 50;
	
	int head;
	if (h != nullptr)
	{
		head = h->m_cycleNo;
	} else
	{
		head = m_hamCycle.GetNodeNo(m_snakes[0].getPosition().x, m_snakes[0].getPosition().y);
	}

	int actualTail;
	if (t != nullptr)
	{
		actualTail = m_hamCycle.GetNodeNo(t->m_x, t->m_y);
	} else
	{
		actualTail = m_hamCycle.GetNodeNo(m_tailBlocks[0].x, m_tailBlocks[0].y);
	}
	
	if (GetDistanceBetweenPoints(head, actualTail) <= minDistanceBetweenHeadAndTail + 1)
	{
		return true;
	}

	auto tail = actualTail - minDistanceBetweenHeadAndTail - 1;
	if (tail < 0)
	{
		tail += m_hamCycle.m_cycle.size();
	}

	auto temp = head;
	auto nextPosNo = newPos->m_cycleNo;
	if (GetDistanceBetweenPoints(head, newPos->m_cycleNo) >= GetDistanceBetweenPoints(head, tail))
	{
		return true;
	}
	
	return false;
}

int Snake::GetDistanceBetweenPoints(const int from, const int to) const
{
	auto distance = to - from;
	while (distance < 0)
	{
		distance += m_hamCycle.m_cycle.size();
	}
	return distance;
}

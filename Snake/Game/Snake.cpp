#include "stdafx.h"

#include "Snake.h"

#include "Hamiltonian/HamiltonianCycle.h"

using namespace sf;

Snake::Snake(RenderWindow& window, HamiltonianCycle& hamCycle, const unsigned cellSize, FoodGenerator& foodGen) : m_window(window), m_hamCycle(hamCycle), m_cellSize(cellSize), m_direction(Direction::right),
														 m_timeToUpdate(50000), m_foodGenerator(foodGen)
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
			for (auto i = m_snakes.size() - 1; i > 0; i--)
			{
				m_snakes[i].setPosition(Vector2f(m_snakes[i - 1].getPosition().x, m_snakes[i - 1].getPosition().y));
			}
		}

		if (m_isAI)
		{
			if (m_path || m_path->m_pathCounter >= m_path->m_pathLength)
			{
				CalculatePath();
			}

			if (!m_path || m_path->m_pathLength == 0)
			{
				//To do 87
			}
		} else
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
	}
}

void Snake::AddCase() // Add 1 more case
{
	RectangleShape cell;
	cell.setSize(Vector2f(static_cast<float>(m_cellSize), static_cast<float>(m_cellSize)));
	cell.setOutlineColor(Color::Black);
	cell.setOutlineThickness(1.0f);

	// First cell is yellow and spawn at the middle of the screen
	if (m_snakes.empty())
	{
		cell.setFillColor(Color::Yellow);
		
		cell.setPosition(m_hamCycle.m_cycle[0]->m_x * m_cellSize, m_hamCycle.m_cycle[0]->m_y * m_cellSize);
	} else
	{
		cell.setFillColor(Color::Green);
		
		if (m_snakes.size() == 1)
		{
			cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x - cell.getSize().x * m_direction.x, m_snakes[m_snakes.size() - 1].getPosition().y - cell.getSize().y * m_direction.y);
		}
		else
		{
			if (m_snakes[m_snakes.size() - 1].getPosition().x == m_snakes[m_snakes.size() - 2].getPosition().x)
			{
				if (m_snakes[m_snakes.size() - 1].getPosition().y - m_snakes[m_snakes.size() - 2].getPosition().y > 0)
				{
					cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x, m_snakes[m_snakes.size() - 1].getPosition().y + 1);
				}
				else
				{
					cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x, m_snakes[m_snakes.size() - 1].getPosition().y - 1);
				}
			}
			else if (m_snakes[m_snakes.size() - 1].getPosition().y == m_snakes[m_snakes.size() - 2].getPosition().y)
			{
				if (m_snakes[m_snakes.size() - 1].getPosition().x - m_snakes[m_snakes.size() - 2].getPosition().x > 0)
				{
					cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x + 1, m_snakes[m_snakes.size() - 1].getPosition().y);
				}
				else
				{
					cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x - 1, m_snakes[m_snakes.size() - 1].getPosition().y);
				}
			} else
			{
				throw std::runtime_error("Failed to add snake cell");
			}
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

	auto* startNode = m_hamCycle.m_cycle[m_hamCycle.GetNodeNo(m_snakes[0].getPosition().x, m_snakes[0].getPosition().y)];
	std::deque<HPath> bigList;

	std::shared_ptr<HPath> winningPath;
	
	bigList.emplace_back(*startNode, *m_hamCycle.m_cycle[m_appleCyclePosition]);

	while (true)
	{
		if (bigList.empty())
		{
			return winningPath;
		}
		auto& currentPath = bigList[0];
		bigList.pop_front();
		if (winningPath && currentPath.m_pathLength >= winningPath->m_pathLength)
		{
			continue;
		}

		if (currentPath.m_distanceToApple == 0) //path has found apple
		{
			if (winningPath == nullptr || currentPath.m_pathLength < winningPath->m_pathLength) {
				winningPath = std::make_shared<HPath>(currentPath);
			}
			continue;
		}

		//if the final node has been visited and the previous visit was a shorter path then fuck this path
		auto finalNodeInPath = currentPath.GetLastNode();

		if (!finalNodeInPath.m_alreadyVisited || currentPath.m_pathLength < finalNodeInPath.m_shortestDistanceToThisPoint)
		{
			//this is the shortest found path to this point
			finalNodeInPath.m_alreadyVisited = true;
			finalNodeInPath.m_shortestDistanceToThisPoint = currentPath.m_pathLength;

			//now we need to add all the paths possible from this node to the bigList
			for (auto* node : finalNodeInPath.m_edges)
			{
				if (OverTakesTail(*node, finalNodeInPath, currentPath.GetSnakeTailPositionAfterFollowingPath(m_snakes.size())))
				{
					if (node->m_cycleNo != finalNodeInPath.m_cycleNo + 1)
					{
						continue;
					}
				}

				auto p = std::make_shared<HPath>(currentPath);
				p->AddToTail(*node);
				if (p->GetLastNode().m_alreadyVisited && p->m_pathLength > p->GetLastNode().m_shortestDistanceToThisPoint)
				{
					continue;
				}
				bigList.push_back(*p);
			}
		}

		//now we need to sort the bigList based on the distances to the apple plus the current distance of the path
		std::sort(bigList.begin(), bigList.end(), [](HPath& a, HPath& b) { return (a.m_distanceToApple + a.m_pathLength) - (b.m_distanceToApple + b.m_pathLength);});
	}
}

bool Snake::OverTakesTail(HNode& newPos, HNode& h, HNode& t)
{
	const auto minDistanceBetweenHeadAndTail = 50;
	const auto head = h.m_cycleNo;

	const auto actualTail = m_hamCycle.GetNodeNo(m_snakes[0].getPosition().x, m_snakes[0].getPosition().y);
	if (GetDistanceBetweenPoints(head, actualTail) <= minDistanceBetweenHeadAndTail + m_snakes.size())
	{
		return true;
	}

	int tail = actualTail - minDistanceBetweenHeadAndTail - m_snakes.size();
	if (tail < 0)
	{
		tail += m_hamCycle.m_cycle.size();
	}

	auto temp = head;
	auto nextPosNo = newPos.m_cycleNo;
	if (GetDistanceBetweenPoints(head, newPos.m_cycleNo) >= GetDistanceBetweenPoints(head, tail))
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

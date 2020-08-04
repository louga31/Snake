#pragma once

class HamiltonianCycle;
using namespace sf;

class Snake
{
public:
	Snake(RenderWindow& window, HamiltonianCycle& hamCycle, unsigned cellSize, FoodGenerator& foodGen);


	std::vector<Vector2f> m_tailBlocks;
	std::vector<RectangleShape> m_snakes;

	// ReSharper disable once CppInconsistentNaming
	bool m_isAI = false;
	
	void Move();
	void Update(Event& event);
	void Draw() const;
	
private:
	RenderWindow& m_window;
	HamiltonianCycle& m_hamCycle;
	unsigned m_cellSize;
	
	Vector2i m_direction = Direction::right;
	
	Clock m_clock;
	int m_timeToUpdate = 50000; // Microsecond
	
	FoodGenerator& m_foodGenerator;

	// Hamiltonian
	std::shared_ptr<HPath> m_path;
	unsigned m_appleCyclePosition;

	void CheckCollision();
	void AddCase();

	// Hamiltonian
	std::shared_ptr<HPath> CalculatePath();
	bool OverTakesTail(HNode* newPos, HNode* h, HNode* t);
	int GetDistanceBetweenPoints(int from, int to) const;
};

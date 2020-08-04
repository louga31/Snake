#pragma once

class HamiltonianCycle;
using namespace sf;

class Snake
{
public:
	Snake(RenderWindow& window, HamiltonianCycle& hamCycle, unsigned cellSize, FoodGenerator& foodGen);

	void Move();
	void Update(Event& event);
	void Draw() const;
	
private:
	RenderWindow& m_window;
	HamiltonianCycle& m_hamCycle;
	unsigned m_cellSize;
	
	Vector2i m_direction;
	
	Clock m_clock;
	int m_timeToUpdate; // Microsecond
	// 
	// ReSharper disable once CppInconsistentNaming
	bool m_isAI = false;
	
	std::vector<RectangleShape> m_snakes;
	FoodGenerator& m_foodGenerator;

	// Hamiltonian
	HPath* m_path = nullptr;
	unsigned m_appleCyclePosition;

	void CheckCollision();
	void AddCase();

	// Hamiltonian
	std::shared_ptr<HPath> CalculatePath();
	bool OverTakesTail(HNode& newPos, HNode& h, HNode& t);
	int GetDistanceBetweenPoints(int from, int to) const;
};

#pragma once

using namespace sf;

class Snake
{
public:
	Snake(RenderWindow& window, unsigned cellSize, FoodGenerator& foodGen);

	void Move();
	void Update(Event& event);
	void Draw() const;
	
private:
	RenderWindow& m_window;
	unsigned m_cellSize;
	Vector2i m_direction;
	Clock m_clock;
	int m_timeToUpdate; // Microsecond
	// ReSharper disable once CppInconsistentNaming
	bool m_isAI = false;
	std::vector<RectangleShape> m_snakes;
	FoodGenerator& m_foodGenerator;

	void CheckCollision();
	void AddCase();
};

// ReSharper disable CppInconsistentNaming
#pragma once

struct Direction
{
	static Vector2i up;
	static Vector2i left;
	static Vector2i right;
	static Vector2i down;
};
Vector2i Direction::up = { 0, -1 };
Vector2i Direction::left = { -1, 0 };
Vector2i Direction::right = { 1, 0 };
Vector2i Direction::down = { 0, 1 };
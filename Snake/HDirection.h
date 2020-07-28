// ReSharper disable CppInconsistentNaming
#pragma once

struct HDirection
{
	int x, y;
};

inline HDirection GetLeftOf(const HDirection d)
{
    if (d.x == 0 && d.y == 1) {
        return HDirection{ 1, 0 };
    }
	if (d.x == 0 && d.y == -1) {
		return HDirection{ -1, 0 };
	}
	if (d.x == 1) {
		return HDirection{ 0, -1 };
	}
	return HDirection{ 0, 1 };
}

inline HDirection getRightOf(const HDirection d) {
    if (d.x == 0 && d.y == 1) {
        return {-1, 0 };
    }
	if (d.x == 0 && d.y == -1) {
		return {1, 0 };
	}
	if (d.x == 1) {
		return {0, 1 };
	}
	return { 0, -1 };
}

////////////////////////////////////////////////////////////
/// \relates HDirection
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two HNode.
///
/// \param left  Left operand (a HDirection)
/// \param right Right operand (a HDirection)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
inline bool operator ==(const HDirection& left, const HDirection& right)
{
	return left.x == right.x && left.y == right.y;
}
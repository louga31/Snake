#pragma once

inline int RandomInt(const int& min, const int& max)
{
    return rand() % (max - min + 1) + min;
}

template<class T>
T& RandomElement(std::vector<T>& elements)
{
	return elements[RandomInt(0, elements.size() - 1)];
}

/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.
bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
*/
template < typename T>
std::pair<bool, int > FindInVector(const std::vector<T>& vecOfElements, const T& element)
{
    std::pair<bool, int > result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
    if (it != vecOfElements.end())
    {
        result.second = distance(vecOfElements.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}

// ReSharper disable once CppInconsistentNaming
inline float dist(const int x1, const int y1, const int x2, const int y2)
{
    return hypot(x2 - x1, y2 - y1);
}

template <typename Cont, typename Pred>
Cont filter(const Cont &container, Pred predicate) {
    Cont result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), predicate);
    return result;
}
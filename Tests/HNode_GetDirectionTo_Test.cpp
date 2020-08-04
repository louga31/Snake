#include "stdafx.h"

#define BOOST_TEST_MODULE HamiltonianTests

#include <boost/test/unit_test.hpp>

#include "../Snake/Hamiltonian/HDirection.h"
#include "../Snake/Hamiltonian/HNode.h"

BOOST_AUTO_TEST_SUITE(HNode_GetDirectionTo_Test)

	BOOST_AUTO_TEST_CASE(DirectionToNegativeX1Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 1,2 };

		const HDirection expectedValue{ -1, 0 };
		
		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToPositiveX1Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 3,2 };

		const HDirection expectedValue{ 1, 0 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToNegativeY1Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 2,1 };
		
		const HDirection expectedValue{ 0, -1 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToPositiveY1Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 2,3 };

		const HDirection expectedValue{ 0, 1 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToNegativeX2Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 1,2 };

		const HDirection expectedValue{ -1, 0 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToPositiveX2Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 4,2 };

		const HDirection expectedValue{ 2, 0 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToNegativeY2Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 2,0 };

		const HDirection expectedValue{ 0, -2 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

	BOOST_AUTO_TEST_CASE(DirectionToPositiveY2Setp)
	{
		const HNode testNode{ 2, 2 };

		const HNode toNode{ 2,4 };

		const HDirection expectedValue{ 0, 2 };

		BOOST_REQUIRE_EQUAL(testNode.GetDirectionTo(toNode), expectedValue);
	}

BOOST_AUTO_TEST_SUITE_END()
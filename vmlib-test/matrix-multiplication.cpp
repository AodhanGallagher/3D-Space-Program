#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// Test case to verify that 4x4 matrix multiplication is working correctly
TEST_CASE( "4x4 matrix by matrix multiplication", "[mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	// Left matrix for multiplication
	constexpr Mat44f testLeft = { {
		3.f, 1.f, 5.f, 2.f,
		4.f, 6.f, 2.f, 1.f,
		7.f, 3.f, 5.f, 2.f,
		8.f, 5.f, 4.f, 2.f
	} };

	// Right matrix for multiplication
	constexpr Mat44f testRight = { {
		7.f, 3.f, 5.f, 9.f,
		3.f, 4.f, 7.f, 8.f,
		1.f, 6.f, 4.f, 2.f,
		4.f, 5.f, 7.f, 9.f
	} };

	using namespace Catch::Matchers;

	// Standard 4x4 matrix-matrix multiplication test
	SECTION( "Standard matrix-matrix multiplication" )
	{
		auto const result = testLeft * testRight;

		REQUIRE_THAT( result(0,0), WithinAbs( 37.f, kEps_ ) );
		REQUIRE_THAT( result(0,1), WithinAbs( 53.f, kEps_ ) );
		REQUIRE_THAT( result(0,2), WithinAbs( 56.f, kEps_ ) );
		REQUIRE_THAT( result(0,3), WithinAbs( 63.f, kEps_ ) );

		REQUIRE_THAT( result(1,0), WithinAbs( 52.f, kEps_ ) );
		REQUIRE_THAT( result(1,1), WithinAbs( 53.f, kEps_ ) );
		REQUIRE_THAT( result(1,2), WithinAbs( 77.f, kEps_ ) );
		REQUIRE_THAT( result(1,3), WithinAbs( 97.f, kEps_ ) );

		REQUIRE_THAT( result(2,0), WithinAbs( 71.f, kEps_ ) );
		REQUIRE_THAT( result(2,1), WithinAbs( 73.f, kEps_ ) );
		REQUIRE_THAT( result(2,2), WithinAbs( 90.f, kEps_ ) );
		REQUIRE_THAT( result(2,3), WithinAbs( 115.f, kEps_ ) );

		REQUIRE_THAT( result(3,0), WithinAbs( 83.f, kEps_ ) );
		REQUIRE_THAT( result(3,1), WithinAbs( 78.f, kEps_ ) );
		REQUIRE_THAT( result(3,2), WithinAbs( 105.f, kEps_ ) );
		REQUIRE_THAT( result(3,3), WithinAbs( 138.f, kEps_ ) );
	}
}

// Test case to verify that 4x4 matrix-vector multiplication is working correctly
TEST_CASE( "4x4 matrix by vector multiplication", "[mat44][vec4]" )
{
	static constexpr float kEps_ = 1e-6f;

	// 4x4 matrix for multiplication
	constexpr Mat44f testMatrix = { {
		3.f, 1.f, 5.f, 2.f,
		4.f, 6.f, 2.f, 1.f,
		7.f, 3.f, 5.f, 2.f,
		8.f, 5.f, 4.f, 2.f
	} };

	// 4 vector for multiplication
	constexpr Vec4f testVector = {
		3.f, 9.f, 6.f, 7.f
	};

	using namespace Catch::Matchers;

	// Standard 4x4 matrix-vector multiplication test
	SECTION( "Standard matrix-vector multiplication" )
	{
		auto const resultVector = testMatrix * testVector;

		REQUIRE_THAT( resultVector[0], WithinAbs( 62.f, kEps_ ) );
		REQUIRE_THAT( resultVector[1], WithinAbs( 85.f, kEps_ ) );
		REQUIRE_THAT( resultVector[2], WithinAbs( 92.f, kEps_ ) );
		REQUIRE_THAT( resultVector[3], WithinAbs( 107.f, kEps_ ) );
	}
}

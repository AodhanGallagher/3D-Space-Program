#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// Test case to verify rotations around the x axis are working as expected
TEST_CASE( "4x4 rotation around x axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_x( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_x( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_x( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_x( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}


// Test case to verify rotations around the y axis are working as expected
TEST_CASE( "4x4 rotation around y axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_y( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_y( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_y( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_y( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}


// Test case to verify rotations around the z axis are working as expected
TEST_CASE( "4x4 rotation around Z axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_z( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_z( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_z( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_z( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}

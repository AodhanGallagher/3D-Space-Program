#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// Test case to verify matrix translations are working as expected
TEST_CASE( "Translations", "[translation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Check that matrix is tranlated along the x-axis properly
	SECTION( "X Translate" )
	{
		auto const identity = make_translation( {5.f, 0.f, 0.f} );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 5.f, kEps_ ) );

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

    // Check that matrix is tranlated along the y-axis properly
	SECTION( "Y Translate" )
	{
		auto const identity = make_translation( {0.f, 7.f, 0.f} );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 7.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

    // Check that matrix is tranlated along the y-axis properly
	SECTION( "Z Translate" )
	{
		auto const identity = make_translation( {0.f, 0.f, 9.f} );

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
		REQUIRE_THAT( identity(2,3), WithinAbs( 9.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

    // Check that matrix is tranlated along all axes properly
	SECTION( "Z Translate" )
	{
		auto const identity = make_translation( {8.f, 6.f, 4.f} );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 8.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 6.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 4.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}
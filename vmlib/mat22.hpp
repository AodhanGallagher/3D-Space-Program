#ifndef MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
#define MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

#include <cmath>

#include "vec2.hpp"

/** Mat22f : 2x2 matrix with floats
 *
 * See comments for Vec2f for some discussion.
 *
 * The matrix is stored in row-major order.
 *
 * Example:
 *   Mat22f identity{ 
 *     1.f, 0.f,
 *     0.f, 1.f
 *   };
 */
struct Mat22f
{
	float _00, _01;
	float _10, _11;
};

// Common operators for Mat22f.
// Note that you will need to implement these yourself.

constexpr
Mat22f operator*( Mat22f const& aLeft, Mat22f const& aRight ) noexcept
{
	// Computing the values for the matrix
	float r00 = aLeft._00*aRight._00 + aLeft._01*aRight._10;
	float r01 = aLeft._00*aRight._01 + aLeft._01*aRight._11;
    float r10 = aLeft._10*aRight._00 + aLeft._11*aRight._10;
    float r11 = aLeft._10*aRight._01 + aLeft._11*aRight._11;

	// Returns a Mat22f with the result of the matrix-matrix multiplication
	return Mat22f{r00, r01, r10, r11};
}

constexpr
Vec2f operator*( Mat22f const& aLeft, Vec2f const& aRight ) noexcept
{
	// Computing the values for the vector
	float r0 = aLeft._00*aRight.x + aLeft._01*aRight.y;
	float r1 = aLeft._10*aRight.x + aLeft._11*aRight.y;
	
	// Returns a Vec2f with the result of the matrix-vector multiplication
	return Vec2f{r0, r1};
}

// Functions:

inline
Mat22f make_rotation_2d( float aAngle ) noexcept
{
	// Computing cos and sin
	float cosTheta = std::cos(aAngle);
    float sinTheta = std::sin(aAngle);

	// Returning the rotation matrix
	return Mat22f{cosTheta, -sinTheta, sinTheta, cosTheta};
}

#endif // MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

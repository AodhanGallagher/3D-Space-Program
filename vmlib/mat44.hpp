#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
	float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
	constexpr
	float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

constexpr Vec4f kIdentity4f = { 0.0f, 0.0f, 0.0f, 1.0f };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*( Mat44f const& aLeft, Mat44f const& aRight ) noexcept
{
	Mat44f result = kIdentity44f;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result(i, j) = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result(i, j) += aLeft(i, k) * aRight(k, j);
            }
        }
    }
    return result;
}

constexpr
Vec4f operator*( Mat44f const& aLeft, Vec4f const& aRight ) noexcept
{
	Vec4f result = kIdentity4f;

    for (int i = 0; i < 4; ++i) {
        result[i] = 0.0f;
        for (int j = 0; j < 4; ++j) {
            result[i] += aLeft(i, j) * aRight[j];
        }
    }
    return result;
}

// Functions:

Mat44f invert( Mat44f const& aM ) noexcept;

inline
Mat44f transpose( Mat44f const& aM ) noexcept
{
	Mat44f ret;
	for( std::size_t i = 0; i < 4; ++i )
	{
		for( std::size_t j = 0; j < 4; ++j )
			ret(j,i) = aM(i,j);
	}
	return ret;
}

inline
Mat44f make_rotation_x( float aAngle ) noexcept
{
	float cosA = std::cos(aAngle);
    float sinA = std::sin(aAngle);

    return Mat44f{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosA, -sinA, 0.0f,
        0.0f, sinA, cosA, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}


inline
Mat44f make_rotation_y( float aAngle ) noexcept
{
    float cosA = std::cos(aAngle);
    float sinA = std::sin(aAngle);

    return Mat44f{
        cosA, 0.0f, sinA, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sinA, 0.0f, cosA, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline
Mat44f make_rotation_z( float aAngle ) noexcept
{
    float cosA = std::cos(aAngle);
    float sinA = std::sin(aAngle);

    return Mat44f{
        cosA, -sinA, 0.0f, 0.0f,
        sinA, cosA, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline
Mat44f make_translation( Vec3f aTranslation ) noexcept
{
    return Mat44f{
        1.0f, 0.0f, 0.0f, aTranslation.x,
        0.0f, 1.0f, 0.0f, aTranslation.y,
        0.0f, 0.0f, 1.0f, aTranslation.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline
Mat44f make_scaling( float aSX, float aSY, float aSZ ) noexcept
{
    return Mat44f{
        aSX, 0.0f, 0.0f, 0.0f,
        0.0f, aSY, 0.0f, 0.0f,
        0.0f, 0.0f, aSZ, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}


inline
Mat44f make_perspective_projection( float aFovInRadians, float aAspect, float aNear, float aFar ) noexcept
{
	float s = 1/(std::tan(aFovInRadians / 2.0f));
	float sx = s/aAspect;
	float sy = s;
	float a = -((aFar+aNear)/(aFar-aNear));
	float b = -2*((aFar*aNear)/(aFar-aNear));

	return Mat44f{
        sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, a, b,
        0.0f, 0.0f, -1.0f, 0.0f
    };
}

inline
Mat44f lookAt(Vec3f cameraPos, Vec3f cameraTarget, Vec3f cameraUp) noexcept
{
    Vec3f cameraDir = normalize(cameraPos - cameraTarget);
    Vec3f cameraRight = cross(cameraUp, cameraDir);
    Vec3f cameraUpAdjusted = cross(cameraDir, cameraRight);

    Mat44f viewMatrix;
    // Set the columns of the matrix using the camera's orientation vectors
    viewMatrix(0, 0) = cameraRight.x;
    viewMatrix(1, 0) = cameraRight.y;
    viewMatrix(2, 0) = cameraRight.z;

    viewMatrix(0, 1) = cameraUpAdjusted.x;
    viewMatrix(1, 1) = cameraUpAdjusted.y;
    viewMatrix(2, 1) = cameraUpAdjusted.z;

    viewMatrix(0, 2) = cameraDir.x;
    viewMatrix(1, 2) = cameraDir.y;
    viewMatrix(2, 2) = cameraDir.z;

    viewMatrix(3, 0) = -dot(cameraRight, cameraPos);
    viewMatrix(3, 1) = -dot(cameraUpAdjusted, cameraPos);
    viewMatrix(3, 2) = -dot(cameraDir, cameraPos);

    viewMatrix(0, 3) = 0.0f;
    viewMatrix(1, 3) = 0.0f;
    viewMatrix(2, 3) = 0.0f;
    viewMatrix(3, 3) = 1.0f;

    return viewMatrix;
}




#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

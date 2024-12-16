#include "cylinder.hpp"
#include <array>

SimpleMeshData make_cube(Vec3f aColor, Mat44f aPreTransform)
{
    std::vector<Vec3f> pos;
    std::vector<Vec3f> norm;

    // Define cube vertices
    std::vector<Vec3f> cubeVertices = {
        Vec3f{-0.5f, -0.5f, -0.5f}, // 0
        Vec3f{0.5f, -0.5f, -0.5f},  // 1
        Vec3f{0.5f, 0.5f, -0.5f},   // 2
        Vec3f{-0.5f, 0.5f, -0.5f},  // 3
        Vec3f{-0.5f, -0.5f, 0.5f},  // 4
        Vec3f{0.5f, -0.5f, 0.5f},   // 5
        Vec3f{0.5f, 0.5f, 0.5f},    // 6
        Vec3f{-0.5f, 0.5f, 0.5f}    // 7
    };

    // Define cube faces with vertices listed in reverse order
    std::vector<std::array<int, 3>> cubeFaces = {
        {2, 1, 0}, {0, 3, 2}, 
        {6, 5, 1}, {1, 2, 6}, 
        {4, 5, 6}, {6, 7, 4}, 
        {3, 0, 4}, {4, 7, 3}, 
        {6, 2, 3}, {3, 7, 6}, 
        {0, 1, 5}, {5, 4, 0}  
    };

    // Compute face normals and assign to vertices
    for (const auto &face : cubeFaces)
    {
        const Vec3f &v0 = cubeVertices[face[0]];
        const Vec3f &v1 = cubeVertices[face[1]];
        const Vec3f &v2 = cubeVertices[face[2]];

        // Compute face normal
        Vec3f faceNormal = normalize(cross(v1 - v0, v2 - v0));

        // Assign normals to vertices of the face
        for (int vertexIndex : face)
        {
            pos.push_back(cubeVertices[vertexIndex]);
            norm.push_back(faceNormal);
        }
    }

    // Apply transformation to positions
    for (auto &p : pos)
    {
        Vec4f p4{p.x, p.y, p.z, 1.f};
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{t.x, t.y, t.z};
    }

    std::vector col( pos.size(), aColor );
    return SimpleMeshData{std::move(pos), std::move(col), std::move(norm)};
}

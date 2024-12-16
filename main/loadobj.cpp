#include "loadobj.hpp"

#include <rapidobj/rapidobj.hpp>

#include "../support/error.hpp"

SimpleMeshData load_wavefront_obj(char const* aPath) {
    // Ask rapidobj to load the requested file
    auto result = rapidobj::ParseFile(aPath);

    if (result.error) {
        // Handle error if unable to load OBJ file
        throw Error("Unable to load OBJ file '%s': %s", aPath, result.error.code.message().c_str());
    }

    // OBJ files can define faces that are not triangles. Triangulate any non-triangular faces.
    rapidobj::Triangulate(result);

    // Convert the OBJ data into a SimpleMeshData structure
    SimpleMeshData ret;

    for (auto const& shape : result.shapes) {
        for (std::size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            auto const& idx = shape.mesh.indices[i];

            // Extract position, normals and textcoords information
            ret.positions.emplace_back(Vec3f{
                result.attributes.positions[idx.position_index * 3 + 0],
                result.attributes.positions[idx.position_index * 3 + 1],
                result.attributes.positions[idx.position_index * 3 + 2]
            });

            ret.normals.emplace_back(Vec3f{
                result.attributes.normals[idx.normal_index * 3 + 0],
                result.attributes.normals[idx.normal_index * 3 + 1],
                result.attributes.normals[idx.normal_index * 3 + 2]
            });

            if (idx.texcoord_index >= 0) {
                ret.texcoords.emplace_back(Vec2f{
                    result.attributes.texcoords[idx.texcoord_index * 2 + 0],
                    result.attributes.texcoords[idx.texcoord_index * 2 + 1]
                });
            }

            // Always triangles, so find the face index by dividing the vertex index by three
            auto const& mat = result.materials[shape.mesh.material_ids[i / 3]];

            // Replicate the material ambient color for each vertex
            ret.colors.emplace_back(Vec3f{
                mat.ambient[0],
                mat.ambient[1],
                mat.ambient[2]
            });
        }
    }

    return ret;
}

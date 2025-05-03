#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "Mesh.h"

namespace GLGame {

class BinaryMeshLoader {
public:
    // Save a mesh to a binary file
    static bool SaveMesh(const std::string& filename, const Mesh& mesh);
    
    // Load a mesh from a binary file
    static bool LoadMesh(const std::string& filename, Mesh& mesh);

private:
    // Binary file format version
    static const uint32_t BINARY_MESH_VERSION = 1;
    
    // File header structure
    struct MeshHeader {
        uint32_t version;
        uint32_t vertexCount;
        uint32_t indexCount;
        uint32_t textureCount;
        bool hasNormals;
        bool hasTangents;
        bool hasBitangents;
        bool hasTexCoords;
    };

    // Texture data structure
    struct TextureData {
        uint32_t width;
        uint32_t height;
        uint32_t channels;
        uint32_t dataSize;
        std::string type;
        std::vector<unsigned char> data;
    };
};

} // namespace GLGame 
#include "BinaryMeshLoader.h"
#include <iostream>
#include <glad/glad.h>

namespace GLGame {

bool BinaryMeshLoader::SaveMesh(const std::string& filename, const Mesh& mesh) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    // Write header
    MeshHeader header;
    header.version = BINARY_MESH_VERSION;
    header.vertexCount = static_cast<uint32_t>(mesh.vertices.size());
    header.indexCount = static_cast<uint32_t>(mesh.indices.size());
    header.textureCount = static_cast<uint32_t>(mesh.textures.size());
    header.hasNormals = !mesh.vertices.empty() && mesh.vertices[0].Normal != glm::vec3(0.0f);
    header.hasTangents = !mesh.vertices.empty() && mesh.vertices[0].Tangent != glm::vec3(0.0f);
    header.hasBitangents = !mesh.vertices.empty() && mesh.vertices[0].Bitangent != glm::vec3(0.0f);
    header.hasTexCoords = !mesh.vertices.empty() && mesh.vertices[0].TexCoords != glm::vec2(0.0f);

    file.write(reinterpret_cast<const char*>(&header), sizeof(MeshHeader));

    // Write vertices
    for (const auto& vertex : mesh.vertices) {
        file.write(reinterpret_cast<const char*>(&vertex.Position), sizeof(glm::vec3));
        if (header.hasNormals) {
            file.write(reinterpret_cast<const char*>(&vertex.Normal), sizeof(glm::vec3));
        }
        if (header.hasTangents) {
            file.write(reinterpret_cast<const char*>(&vertex.Tangent), sizeof(glm::vec3));
        }
        if (header.hasBitangents) {
            file.write(reinterpret_cast<const char*>(&vertex.Bitangent), sizeof(glm::vec3));
        }
        if (header.hasTexCoords) {
            file.write(reinterpret_cast<const char*>(&vertex.TexCoords), sizeof(glm::vec2));
        }
        file.write(reinterpret_cast<const char*>(&vertex.boneIds), sizeof(glm::ivec4));
        file.write(reinterpret_cast<const char*>(&vertex.boneWeights), sizeof(glm::vec4));
    }

    // Write indices
    file.write(reinterpret_cast<const char*>(mesh.indices.data()), 
               mesh.indices.size() * sizeof(unsigned int));

    // Write textures
    for (const auto& texture : mesh.textures) {
        // Get texture data from OpenGL
        int width, height, channels;
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &channels);

        // Read texture data
        std::vector<unsigned char> data(width * height * 4); // Always read as RGBA
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

        // Write texture metadata
        uint32_t typeLength = static_cast<uint32_t>(texture.type.length());
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(uint32_t));
        file.write(texture.type.c_str(), typeLength);

        // Write texture dimensions and data
        uint32_t textureWidth = static_cast<uint32_t>(width);
        uint32_t textureHeight = static_cast<uint32_t>(height);
        uint32_t textureChannels = 4; // Always save as RGBA
        uint32_t dataSize = static_cast<uint32_t>(data.size());

        file.write(reinterpret_cast<const char*>(&textureWidth), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(&textureHeight), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(&textureChannels), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(uint32_t));
        file.write(reinterpret_cast<const char*>(data.data()), dataSize);
    }

    return true;
}

bool BinaryMeshLoader::LoadMesh(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return false;
    }

    // Read header
    MeshHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(MeshHeader));

    if (header.version != BINARY_MESH_VERSION) {
        std::cerr << "Unsupported binary mesh version: " << header.version << std::endl;
        return false;
    }

    // Read vertices
    mesh.vertices.resize(header.vertexCount);
    for (auto& vertex : mesh.vertices) {
        file.read(reinterpret_cast<char*>(&vertex.Position), sizeof(glm::vec3));
        if (header.hasNormals) {
            file.read(reinterpret_cast<char*>(&vertex.Normal), sizeof(glm::vec3));
        }
        if (header.hasTangents) {
            file.read(reinterpret_cast<char*>(&vertex.Tangent), sizeof(glm::vec3));
        }
        if (header.hasBitangents) {
            file.read(reinterpret_cast<char*>(&vertex.Bitangent), sizeof(glm::vec3));
        }
        if (header.hasTexCoords) {
            file.read(reinterpret_cast<char*>(&vertex.TexCoords), sizeof(glm::vec2));
        }
        file.read(reinterpret_cast<char*>(&vertex.boneIds), sizeof(glm::ivec4));
        file.read(reinterpret_cast<char*>(&vertex.boneWeights), sizeof(glm::vec4));
    }

    // Read indices
    mesh.indices.resize(header.indexCount);
    file.read(reinterpret_cast<char*>(mesh.indices.data()), 
              header.indexCount * sizeof(unsigned int));

    // Read textures
    mesh.textures.clear();
    for (uint32_t i = 0; i < header.textureCount; ++i) {
        // Read texture type
        uint32_t typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(uint32_t));
        std::string type(typeLength, '\0');
        file.read(&type[0], typeLength);

        // Read texture dimensions and data
        uint32_t width, height, channels, dataSize;
        file.read(reinterpret_cast<char*>(&width), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&height), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&channels), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(uint32_t));

        std::vector<unsigned char> data(dataSize);
        file.read(reinterpret_cast<char*>(data.data()), dataSize);

        // Create OpenGL texture
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Create and add texture
        mesh.textures.emplace_back(textureID, type, "");
    }

    // Create a default material
    mesh.material = Material();

    // Setup the mesh after loading
    mesh.setupMesh();

    return true;
}

} // namespace GLGame 
#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "../Common/OpenGL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "stb/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Shader.h"
#include "../Material/Material.h"

#include "Animator.h"
#include "../Object/Transform.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

namespace GLGame {
    class Model
    {
    public:
        // model data 
        vector<GLGame::Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<Mesh>    meshes;
        string directory;
        bool gammaCorrection;
        glm::vec3 scale;

        GLGame::Transformation transform;

        GLGame::Bone skeleton;
        GLGame::AnimationNode animation;
        GLGame::Animator animator;

        // Default constructor
        Model() : gammaCorrection(false) {}

        // constructor, expects a filepath to a 3D model.
        Model(string const& path, glm::vec3 scale = glm::vec3(1.0f), bool gamma = false) : scale(scale), gammaCorrection(gamma)
        {
            loadModel(path);
        }

        // draws the model, and thus all its meshes
        void Draw(Shader& shader)
        {
            animator.animateSkeleton(shader, animation, skeleton);

            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(shader);
        }

    private:
        inline glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat) {
            glm::mat4 m;
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    m[x][y] = mat[y][x];
                }
            }
            return m;
        }

        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const& path)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace); // aiProcess_FlipUVs, aiProcess_MakeLeftHanded
            // check for errors
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }
            // retrieve the directory path of the filepath
            directory = path.substr(0, path.find_last_of('/'));

            animator.load(scene, animation);

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene)
        {
            std::cout << "Pocet meshes: " << node->mNumMeshes << std::endl;

            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            // data to fill
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;
            GLGame::Material meshMaterial = GLGame::Material();

            std::cout << "number of bones: " << mesh->mNumBones << std::endl;
            std::cout << "number of verticies: " << mesh->mNumVertices;

            // walk through each of the mesh's vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }
                // texture coordinates
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }

            //load boneData to vertices
            std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
            std::vector<unsigned int> boneCounts;
            boneCounts.resize(vertices.size(), 0);

            //loop through each bone
            for (unsigned int i = 0; i < mesh->mNumBones; i++) {
                aiBone* bone = mesh->mBones[i];
                glm::mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);
                boneInfo[bone->mName.C_Str()] = std::make_pair(i, m);

                //loop through each vertex that have that bone
                for (int j = 0; j < bone->mNumWeights; j++) {
                    unsigned int id = bone->mWeights[j].mVertexId;
                    float weight = bone->mWeights[j].mWeight;
                    boneCounts[id]++;
                    switch (boneCounts[id]) {
                    case 1:
                        vertices[id].boneIds.x = i;
                        vertices[id].boneWeights.x = weight;
                        break;
                    case 2:
                        vertices[id].boneIds.y = i;
                        vertices[id].boneWeights.y = weight;
                        break;
                    case 3:
                        vertices[id].boneIds.z = i;
                        vertices[id].boneWeights.z = weight;
                        break;
                    case 4:
                        vertices[id].boneIds.w = i;
                        vertices[id].boneWeights.w = weight;
                        break;
                    default:
                        std::cout << "err: unable to allocate bone to vertex" << std::endl;
                        break;

                    }
                }
            }

            readSkeleton(skeleton, scene->mRootNode, boneInfo);

            //normalize weights to make all weights sum 1
            for (int i = 0; i < vertices.size(); i++) {
                glm::vec4& boneWeights = vertices[i].boneWeights;
                float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
                if (totalWeight > 0.0f) {
                    vertices[i].boneWeights = glm::vec4(
                        boneWeights.x / totalWeight,
                        boneWeights.y / totalWeight,
                        boneWeights.z / totalWeight,
                        boneWeights.w / totalWeight
                    );
                }
            }

            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // 1. diffuse maps
            // vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse", scene);
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "material.diffuse", scene);
            Texture diffuseTexture = diffuseMaps.front();
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // 2. specular maps
            //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular", scene);
            //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "material.normal", scene);
            Texture normalTexture = normalMaps.front();
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
            //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            std::vector<Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "material.metallic", scene);
            GLGame::Texture* metallic = nullptr;
            if (!metallicMaps.empty()) {
                Texture metallicTexture = metallicMaps.front();
                metallic = new GLGame::Texture(metallicTexture.id, metallicTexture.type, "");
                textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
            }

            // std::vector<Texture> roughness = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "material.roughness", scene);
            // textures.insert(textures.end(), roughness.begin(), roughness.end());

            //std::vector<Texture> ao = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "material.ao", scene);
            //textures.insert(textures.end(), ao.begin(), ao.end());

            // return a mesh object created from the extracted mesh data
            std::cout << "\npocet textur vo vectore: " << textures.size();

            std::vector<GLGame::Texture*> texturesToMaterial;
            std::string path = "";

            texturesToMaterial.push_back(new GLGame::Texture(diffuseTexture.id, diffuseTexture.type, path));
            texturesToMaterial.push_back(new GLGame::Texture(normalTexture.id, normalTexture.type, path));
            texturesToMaterial.push_back(metallic);

            meshMaterial.setTextures(texturesToMaterial);

            // Create a Mesh object with the correct parameter types
            return Mesh(vertices, indices, textures, meshMaterial);
        }

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
        {
            vector<Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // Check if texture is embedded
                const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if (!skip)
                {   // if texture hasn't been loaded already, load it
                    GLGame::Texture texture(
                        TextureFromFile(str.C_Str(), this->directory, false, embeddedTexture),
                        typeName,
                        str.C_Str()
                    );
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
                }
            }
            return textures;
        }

        unsigned int TextureFromFile(const char* path, const string& directory, bool gamma, const aiTexture* embeddedTexture)
        {
            string filename = string(path);
            filename = directory + '/' + filename;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, channels;

            size_t dataSize = embeddedTexture->mWidth;
            const unsigned char* compressedData = reinterpret_cast<const unsigned char*>(embeddedTexture->pcData);
            unsigned char* imageData = nullptr;

            if (embeddedTexture->mHeight == 0)
            {
                imageData = stbi_load_from_memory(compressedData, dataSize, &width, &height, &channels, 4);
            }
            else
            {
                imageData = stbi_load_from_memory(compressedData, embeddedTexture->mWidth * embeddedTexture->mHeight, &width, &height, &channels, 0);
            }

            // load from not compressed JPEG/PNG
            // unsigned char* data = stbi_load("../../../resources/prisoner_diffuse.PNG", &width, &height, &nrComponents, 0);

            if (imageData)
            {
                GLenum format;
                if (channels == 1)
                    format = GL_RED;
                else if (channels == 3)
                    format = GL_RGB;
                else if (channels == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(imageData);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(imageData);
            }

            return textureID;
        }

        // TODO Skeleton loader/reader?
        bool readSkeleton(GLGame::Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable) {

            if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end()) { // if node is actually a bone
                boneOutput.name = node->mName.C_Str();
                boneOutput.id = boneInfoTable[boneOutput.name].first;
                boneOutput.offset = boneInfoTable[boneOutput.name].second;

                for (int i = 0; i < node->mNumChildren; i++) {
                    GLGame::Bone child;
                    readSkeleton(child, node->mChildren[i], boneInfoTable);
                    boneOutput.children.push_back(child);
                }
                return true;
            }
            else { // find bones in children
                for (int i = 0; i < node->mNumChildren; i++) {
                    if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable)) {
                        return true;
                    }

                }
            }
            return false;
        }
    };
}


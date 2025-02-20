#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 5) in ivec4 aBoneIDs; // Bone IDs influencing this vertex
layout(location = 6) in vec4 aWeights; // Weights for each bone

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransforms[100]; // Array of bone transformations

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec4 totalNormal = vec4(0.0);

    for (int i = 0; i < 4; i++) {
        if (aBoneIDs[i] == -1) continue; // Skip unused bones
        mat4 boneTransform = boneTransforms[aBoneIDs[i]];
        totalPosition += boneTransform * vec4(aPos, 1.0) * aWeights[i];
        totalNormal += boneTransform * vec4(aNormal, 0.0) * aWeights[i];
    }

    FragPos = vec3(model);
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalMatrix * aNormal;
    Tangent = normalMatrix * aTangent;
    Bitangent = normalMatrix * aBitangent;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
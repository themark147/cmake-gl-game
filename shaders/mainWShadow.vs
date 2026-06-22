#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 boneWeights;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform mat4 bone_transforms[100];

void main()
{
    mat4 boneTransform  =  mat4(1.0);
	boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
	boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
	boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
	boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;

    vec4 pos = boneTransform * vec4(aPos, 1.0);
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    mat3 nm = normalMatrix * mat3(boneTransform);

    Normal = nm * aNormal;
    Tangent = nm * aTangent;
    Bitangent = nm * aBitangent;

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    gl_Position = projection * view * model * pos;
}
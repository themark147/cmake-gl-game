#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 boneWeights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform mat4 bone_transforms[100];

void main()
{
    mat4 boneTransform  =  mat4(1.0);
	boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
	boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
	boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
	boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;

    vec4 pos = boneTransform * vec4(aPos, 1.0);

    gl_Position = lightSpaceMatrix * model * pos;
    
    // gl_Position = projection * view * model * pos;
}
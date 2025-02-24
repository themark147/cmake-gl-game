#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 5) in vec4 boneIds;
layout(location = 6) in vec4 boneWeights;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 bone_transforms[50];
out vec4 bw;

void main()
{

	vec4 totalPosition = vec4(0.0);

    for (int i = 0; i < 46; i++) {
        if (boneIds[i] == -1) break; // No more bones affecting this vertex
        vec4 localPosition = bone_transforms[int(boneIds[i])] * vec4(aPos, 1.0);
        totalPosition += localPosition * boneWeights[i];
    }

    gl_Position = projection * view * model * totalPosition;

   // bw = vec4(0);
	//if(int(boneIds.x) == 1)
	//bw.z = boneIds.x;
	// boneWeights = normalize(boneWeights);
   // mat4 boneTransform  =  mat4(0.0);
	//boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
	//boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
//	boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
	//boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;

    //vec4 pos = boneTransform * vec4(aPos, 1.0);
    
  //  TexCoords = aTexCoords;
    FragPos = vec3(model);

  //  mat3 normalMatrix = mat3(transpose(inverse(model * boneTransform)));

    //Normal = normalMatrix * aNormal;
    //Tangent = normalMatrix * aTangent;
   // Bitangent = normalMatrix * aBitangent;
    
   // gl_Position = projection * view * model * vec4(aPos, 1.0); // * boneTransform
    

    /*vec4 pos =boneTransform * vec4(position, 1.0);
		gl_Position = view_projection_matrix * model_matrix * boneTransform * vec4(aPos, 1.0);
		v_pos = vec3(model_matrix * boneTransform * pos);
		tex_cord = uv;
		v_normal = mat3(transpose(inverse(model_matrix * boneTransform))) * normal;
		v_normal = normalize(v_normal);*/
}
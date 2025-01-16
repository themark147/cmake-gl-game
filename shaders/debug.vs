#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoord;
//in uint vertexColor;

// Out variables
// out vec4 vertexColorOut;
out vec2 TexCoord;

uniform mat4 model; // not used anymore
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * aPos;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	// vertexColorOut = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
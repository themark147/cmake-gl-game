#pragma once

#include <string>

std::string debug_fragment_shader = R"(

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
//in vec4 vertexColorOut;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

vec2 pitch  = vec2(25, 25);

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	/*if (int(mod(gl_FragCoord.x, pitch[0])) == 0 || int(mod(gl_FragCoord.y, pitch[1])) == 0)
    {
        FragColor = vec4(0.7, 0.3, 0.5, 1.0);
    } else {
        FragColor = vec4(0);
    }*/

	FragColor = vec4(1.0, 0.7, 0.5, 1.0);
}
)";

std::string debug_vertex_shader = R"(
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
)";
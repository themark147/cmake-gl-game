#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 a_texCoord;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;
out vec3 v_pos;

uniform mat4 model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_time;

// Wave parameters
uniform float u_waveAmplitude;
uniform float u_waveFrequency;
uniform float u_waveSpeed;

void main() {
    v_texCoord = a_texCoord;
    
    // Calculate wave displacement (Y is up)
    vec3 pos = a_position;
    
    // Multiple wave sources for more interesting effect
    float wave1 = sin(pos.x * u_waveFrequency + u_time * u_waveSpeed) * u_waveAmplitude;
    float wave2 = sin(pos.z * u_waveFrequency * 0.7 + u_time * u_waveSpeed * 1.3) * u_waveAmplitude * 0.5;
    float wave3 = cos(length(pos.xz) * u_waveFrequency * 0.5 + u_time * u_waveSpeed * 0.8) * u_waveAmplitude * 0.3;
    
    pos.y += wave1 + wave2 + wave3;
    
    // Calculate normal by computing partial derivatives
    float delta = 0.01;
    
    // Sample nearby points for derivative calculation
    float heightRight = sin((pos.x + delta) * u_waveFrequency + u_time * u_waveSpeed) * u_waveAmplitude
                      + sin(pos.z * u_waveFrequency * 0.7 + u_time * u_waveSpeed * 1.3) * u_waveAmplitude * 0.5
                      + cos(length(vec2(pos.x + delta, pos.z)) * u_waveFrequency * 0.5 + u_time * u_waveSpeed * 0.8) * u_waveAmplitude * 0.3;
    
    float heightForward = sin(pos.x * u_waveFrequency + u_time * u_waveSpeed) * u_waveAmplitude
                        + sin((pos.z + delta) * u_waveFrequency * 0.7 + u_time * u_waveSpeed * 1.3) * u_waveAmplitude * 0.5
                        + cos(length(vec2(pos.x, pos.z + delta)) * u_waveFrequency * 0.5 + u_time * u_waveSpeed * 0.8) * u_waveAmplitude * 0.3;
    
    // Compute tangent vectors (Y is up, so tangents are in XZ plane)
    vec3 tangent = normalize(vec3(delta, heightRight - pos.y, 0.0));
    vec3 bitangent = normalize(vec3(0.0, heightForward - pos.y, delta));
    
    // Normal is cross product of tangent and bitangent
    vec3 normal = normalize(cross(tangent, bitangent));
    
    // Transform normal to world space
    v_normal = mat3(transpose(inverse(model))) * normal;
    
    // Fragment position in world space
    v_fragPos = vec3(model * vec4(pos, 1.0));
    v_pos = pos;
    
    gl_Position = u_projection * u_view * model * vec4(pos, 1.0);
}
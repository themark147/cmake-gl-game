#version 330 core

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;
in vec3 v_pos;

out vec4 FragColor;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;
uniform float u_time;

void main() {
        // Normalize the normal vector
    vec3 norm = normalize(v_normal);
    
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * u_lightColor;
    
    // Diffuse lighting
    vec3 lightDir = normalize(u_lightPos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;
    
    // Specular lighting
    float specularStrength = 0.8;
    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * u_lightColor;
    
    // Add some color variation based on wave height (now using Y)
    vec3 baseColor = u_objectColor;
    baseColor += vec3(0.1, 0.2, 0.3) * (v_pos.y * 2.0);
    
    // Combine lighting
    vec3 result = (ambient + diffuse + specular) * baseColor;
    
    FragColor = vec4(result, 1.0);
}
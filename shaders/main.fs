#version 330 core

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

struct Material {
    sampler2D diffuse; // albedo
    sampler2D normal;
}; 

// Camera position
uniform vec3 viewPos;

// Light information
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDir;

uniform Material material;

const float PI = 3.14159265359;

// Function to calculate the normal from the normal map
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, TexCoords).rgb * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T   = normalize(Tangent);
    vec3 B   = normalize(Bitangent);
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// Geometry Function (Schlick GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// Geometry Function (Smith)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel Equation (Schlick approximation)
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 sRGBToLinear(vec3 color)
{
    return pow(color, vec3(2.2));
}

void main()
{
    // Sample textures
    vec3 albedo = sRGBToLinear(texture(material.diffuse, TexCoords).rgb);
    float metallic = 0;
    float roughness = 0.859;
    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);

    // Calculate reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation
    vec3 Lo = vec3(0.0);

    // Directional light calculation
    vec3 L = normalize(-lightDir);
    vec3 H = normalize(V + L);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // For energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light);
    // to preserve this relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // Multiply kD by the inverse metalness such that only non-metals have diffuse lighting
    kD *= 1.0 - metallic;

    // Scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // Add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * lightColor * NdotL * 5.0;

    // Ambient lighting (simple approximation)
    vec3 ambient = vec3(0.03) * albedo;

    vec3 color = ambient + Lo;

    // Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
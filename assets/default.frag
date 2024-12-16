#version 430

// Input attributes
in vec3 v2fNormal;
in vec2 v2fTexCoord;
in vec3 fragPos;

// Diffuse and Ambient for uniform light 
layout(location = 2) uniform vec3 uLightDir; // should be normalized! ||uLightDir|| = 1
layout(location = 3) uniform vec3 uLightDiffuse;
layout(location = 4) uniform vec3 uSceneAmbient;


// Point 1
layout(location = 5) uniform vec3 pointLightPos1;
layout(location = 6) uniform vec3 pointLightViewPos1;

// Point 2
layout(location = 7) uniform vec3 pointLightPos2;
layout(location = 8) uniform vec3 pointLightViewPos2;

// Point 3
layout(location = 9) uniform vec3 pointLightPos3;
layout(location = 10) uniform vec3 pointLightViewPos3;


// Fragment shader outputs
layout(binding = 0) uniform sampler2D uTexture;
layout(location = 0) out vec3 oColor;


void main()
{
    // original shader model
    vec3 normal = normalize(v2fNormal);
    float nDotL = max(0.0, dot(normal, uLightDir));
    vec3 texColor = texture(uTexture, v2fTexCoord).rgb;


    // Shader model for point 1
    // Ambient
    vec3 color1 = vec3(1.0, 0.0, 0.0);

    // Diffuse
    vec3 lightDir1 = normalize(pointLightPos1 - fragPos);
    vec3 normal1 = normalize(v2fNormal);
    float diff1 = max(dot(lightDir1, normal1), 0.0);

    // Distance-based attenuation
    float distance1 = length(pointLightPos1 - fragPos);
    float attenuation1 = 1.0 / (distance1 * distance1);
    vec3 diffuse1 = diff1 * color1 * attenuation1 * 10;

    // Specular
    vec3 viewDir1 = normalize(pointLightViewPos1 - fragPos); 
    float spec1 = 0.0;
    vec3 halfwayDir1 = normalize(lightDir1 + viewDir1);
    spec1 = pow(max(dot(normal1, halfwayDir1), 0.0), 32.0);
    vec3 specular1 = vec3(0.3) * spec1 * attenuation1;
    // Get the color for point1
    vec3 point1 = vec3(uSceneAmbient + diffuse1 + specular1);

    // Shader model for point 2
    // Ambient
    vec3 color2 = vec3(0.0, 0.0, 1.0);

    // Diffuse
    vec3 lightDir2 = normalize(pointLightPos2 - fragPos);
    vec3 normal2 = normalize(v2fNormal);
    float diff2 = max(dot(lightDir2, normal2), 0.0);

    // Distance-based attenuation
    float distance2 = length(pointLightPos2 - fragPos);
    float attenuation2 = 1.0 / (distance2 * distance2);
    vec3 diffuse2 = diff2 * color2 * attenuation2 * 10;

    // Specular
    vec3 viewDir2 = normalize(pointLightViewPos2 - fragPos); 
    float spec2 = 0.0;
    vec3 halfwayDir2 = normalize(lightDir2 + viewDir2);
    spec2 = pow(max(dot(normal2, halfwayDir2), 0.0), 32.0);
    vec3 specular2 = vec3(0.3) * spec2 * attenuation2;
    // Get the color for point2
    vec3 point2 = vec3(uSceneAmbient + diffuse2 + specular2);



    // Shader model for point 3
    // Ambient
    vec3 color3 = vec3(1.0, 1.0, 1.0);

    // Diffuse
    vec3 lightDir3 = normalize(pointLightPos3 - fragPos);
    vec3 normal3 = normalize(v2fNormal);
    float diff3 = max(dot(lightDir3, normal3), 0.0);

    // Distance-based attenuation
    float distance3 = length(pointLightPos3 - fragPos);
    float attenuation3 = 1.0 / (distance3 * distance3);
    vec3 diffuse3 = diff3 * color3 * attenuation3 * 10;

    // Specular
    vec3 viewDir3 = normalize(pointLightViewPos3 - fragPos); 
    float spec3 = 0.0;
    vec3 halfwayDir3 = normalize(lightDir3 + viewDir3);
    spec3 = pow(max(dot(normal3, halfwayDir3), 0.0), 32.0);
    vec3 specular3 = vec3(0.3) * spec3 * attenuation3;
    // Get the color for point2
    vec3 point3 = vec3(uSceneAmbient + diffuse3 + specular3);

    // Compute the final color 
    oColor =  ((point1 + point2 + point3)) * texColor;
}

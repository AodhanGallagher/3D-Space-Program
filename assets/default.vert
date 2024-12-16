#version 430

// Input attributes
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColor;
layout(location = 2) in vec3 iNormal;
layout(location = 3) in vec2 iTexCoord;

// Uniforms
layout(location = 0) uniform mat4 uProjCameraWorld;
layout(location = 1) uniform mat3 uNormalMatrix;

// Output attributes
out vec3 v2fNormal;
out vec2 v2fTexCoord;
out vec3 fragPos;

void main()
{
    fragPos = iPosition;
    // Copy input color to the output color attribute.
    v2fTexCoord = iTexCoord;
    v2fNormal = normalize(uNormalMatrix * iNormal);

    // Transform the input position with the uniform matrix
    gl_Position = uProjCameraWorld * vec4(iPosition, 1.0);
}

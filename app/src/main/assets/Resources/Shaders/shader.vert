#version 300 es
precision mediump float;
uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColour;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in mat4 inTransform;

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 fragPos;

void main() {
    vec4 fragPos4 = inTransform * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * fragPos4;

    fragColour = inColour;
    fragTexCoord = inTexCoord;
    normal = vec3(inTransform * vec4(inNormal, 0.0));
    fragPos = vec3(fragPos4) / fragPos4.w;
}
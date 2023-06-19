#version 330

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragPos;
out vec4 fragColor;
out vec2 fragUV;

uniform mat4 cameraMatrix;
uniform float frames;

void main()
{
    float time = frames / 10.0;
    float strength1 = 15.5;
    float strength2 = 8.5;

    gl_Position.xy = (cameraMatrix * vec4(vertexPosition.x + strength1 * vertexUV.y * cos(time / 2) + (1 - vertexUV.y) * strength2 * cos(time / 2 - 1), vertexPosition.y, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragPos = vertexPosition;
    fragColor = vertexColor;
    fragUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}
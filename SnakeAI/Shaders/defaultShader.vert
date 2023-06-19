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
    gl_Position.xy = (cameraMatrix * vec4(vertexPosition.x + frames - frames, vertexPosition.y, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragPos = vertexPosition;
    fragColor = vertexColor;
    fragUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}
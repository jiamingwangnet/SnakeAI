#version 330

in vec2 fragPos;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    vec4 texColor = fragColor * texture(textureSampler, fragUV);

    color = texColor;
}
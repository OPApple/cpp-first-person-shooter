#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 scale;
uniform vec2 pos;

void main()
{
    // Static position - arms stay at bottom center
    vec2 screenPos = aPos * scale + pos;
    gl_Position = vec4(screenPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
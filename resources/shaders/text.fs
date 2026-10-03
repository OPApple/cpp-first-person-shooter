#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor; 

void main () {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    vec4 color_ = vec4(textColor, 1.0) * sampled;
    if (color_.a < 0.075)
        discard;
    color = color_;
}
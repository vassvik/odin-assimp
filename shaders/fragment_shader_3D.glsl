#version 440 core

in vec3 position;
in vec3 normal;

uniform float time;
uniform float opacity;

out vec4 color;

void main() {
    color.xyz = 0.5 + 0.5*normal;
    color.w = opacity;
}


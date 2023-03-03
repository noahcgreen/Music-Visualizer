#version 410 core

in vec2 position;
uniform mat4 model;

void main() {
    gl_Position = model * vec4(position, 0.0, 1.0);
}

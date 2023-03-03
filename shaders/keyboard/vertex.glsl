#version 410 core

in vec3 position;
in vec3 color;
uniform mat4 model;
out vec3 fColor;

void main() {
    gl_Position = model * vec4(position, 1.0);
    fColor = color;
}

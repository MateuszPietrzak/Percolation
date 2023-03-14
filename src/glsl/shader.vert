#version 330 core

layout (location = 0) in vec2 input_pos;

out vec2 coords;

void main() {
    coords = (input_pos + 1.0) / 2;
    gl_Position = vec4(input_pos, 1.0, 1.0);
}
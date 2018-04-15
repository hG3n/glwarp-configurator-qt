#version 330 core
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_color;

uniform mat4 mvp;

out vec3 fragment_color;

void main() {
    gl_Position = mvp* vec4(vertex_position_modelspace,1.0f);
    fragment_color = vertex_color;
}

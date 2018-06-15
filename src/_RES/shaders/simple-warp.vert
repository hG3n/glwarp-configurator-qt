#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
    gl_PointSize = 8.0f;

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertex_position_modelspace,1);

    // UV of the vertex. No special space for this one.
    UV = vertex_uv;
}

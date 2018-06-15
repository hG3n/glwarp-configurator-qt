#version 330 core
in highp vec3 fragment_color;

out highp vec4 color;

void main() {
    color = vec4(fragment_color, 1.0);
//    color = vec4(1.0,1.0,1.0, 1.0);
}

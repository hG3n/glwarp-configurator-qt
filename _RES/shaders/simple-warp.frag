#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

void main() {
    // since the texture uv origin is in the lower left corner and the images origin in the upper left
    // we need to flip the UV's y-coordinate
    // furthermore gl's internal format converts to bgra anyway, therefor . bgra as color output

//    vec2 coord = vec2(UV.x, 1.0f - UV.y);
    color = texture( texture_sampler, vec2(UV.x, 1.0f- UV.y)).bgra;
    //	color = texture( myTextureSampler, vec2(UV.x, 1.0f- UV.y)).rgba;
    //color = vec4(200.0, 0.0, 0.0, 1.0).rgba;
}

#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float time;
const vec2 resolution = vec2(1000, 1000); // e.g., (1920.0, 1080.0)
const float scale = 2;      // e.g., 4.0

void main()
{
    // --- 1. Calculate the Pixelated Screen Position ---

    // a) Find the raw screen position
    vec2 screen_pos = gl_FragCoord.xy;

    // b) Divide by the scale factor to get the low-resolution position
    vec2 pixel_pos = screen_pos / scale;

    // c) Use 'floor' to snap all fragments within the same 4x4 block
    // to the same position, giving a unified wobble to the block.
    vec2 block_pos = floor(pixel_pos);


    // --- 2. Calculate Dynamic Offset (using the pixelated position) ---

    float wobble_frequency = 5.0;
    float wobble_amplitude = 2.0;

    // Use 'block_pos' instead of 'screen_pos'
    float offset_x = sin(time * wobble_frequency + block_pos.y * 0.1) * wobble_amplitude;
    float offset_y = cos(time * wobble_frequency + block_pos.x * 0.1) * wobble_amplitude;


    // --- 3. Convert Pixel Offset to UV Coordinate Space ---

    // The offset is 1 image pixel wide, so we normalize it by the
    // original, *unscaled* width/height (which is u_resolution / u_scale).

    vec2 low_res = resolution / scale; // e.g., (1920/4, 1080/4) = (480, 270)
    vec2 offset_pixels = vec2(offset_x, offset_y);

    // Normalize the pixel offset by the low resolution
    vec2 normalized_offset = offset_pixels / low_res;


    // --- 4. Sample Texture with Wobbled Coordinates ---
    vec2 wobbled_texcoord = TexCoords + normalized_offset;


    // --- 5. Output Final Color ---
    FragColor = texture(screenTexture, wobbled_texcoord);
}
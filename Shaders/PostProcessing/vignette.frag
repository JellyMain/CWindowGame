#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// --- Vignette Controls ---
// The radius of the "clear" (un-darkened) center.
// 0.0 = tiny center, 1.0 = touches the screen edges.
uniform float vignetteRadius = 0.6;

// How soft the transition from clear to dark is.
// 0.01 = very sharp edge, 0.5 = soft, gradual edge.
uniform float vignetteSoftness = 0.4;

void main()
{
    // 1. Get the original pixel color from the screen texture
    vec4 color = texture(screenTexture, TexCoords);

    // 2. Calculate the distance of the current pixel from the center
    // The center in TexCoords is (0.5, 0.5)
    float dist = distance(TexCoords, vec2(0.5));

    // 3. Calculate the vignette factor (1.0 in center, 0.0 at edges)
    // smoothstep(edge0, edge1, x) transitions from 0.0 to 1.0 as x goes from edge0 to edge1.
    // We reverse the edges (edge0 > edge1) to transition from 1.0 down to 0.0.
    //
    //    - If dist < (vignetteRadius - vignetteSoftness), vignette = 1.0 (full brightness)
    //    - If dist > vignetteRadius, vignette = 0.0 (full darkness)
    //    - If dist is between them, it smoothly blends.
    float vignette = smoothstep(vignetteRadius, vignetteRadius - vignetteSoftness, dist);

    // 4. Apply the vignette by multiplying the color's RGB values
    // We clamp to be safe, ensuring the value is never below 0.
    FragColor = vec4(color.rgb * clamp(vignette, 0.0, 1.0), color.a);
}
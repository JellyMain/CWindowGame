#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

// Your background texture
uniform sampler2D ourTexture;

// Balatro-style uniforms (set from CPU)
uniform vec2 iResolution;      // viewport resolution (e.g., 1920, 1080)
uniform float iTime;           // elapsed time in seconds
uniform bool is_rotating = false;
uniform float spin_rotation_speed = 2.0;
uniform float move_speed = 7.0;
uniform vec2 offset = vec2(0.0, 0.0);
uniform vec4 colour_1 = vec4(0.871, 0.267, 0.231, 1.0);
uniform vec4 colour_2 = vec4(0.0, 0.42, 0.706, 1.0);
uniform vec4 colour_3 = vec4(0.086, 0.137, 0.145, 1.0);
uniform float contrast = 3.5;
uniform float lighting = 0.4;
uniform float spin_amount = 0.25;
uniform float pixel_filter = 740.0;

#define SPIN_EASE 1.0
#define TIME iTime
#define screenSize iResolution

vec4 effect(vec2 screenSize , vec2 screen_coords) {
    float pixel_size = length(screenSize) / pixel_filter;
    vec2 uv = (floor(screen_coords * (1.0 / pixel_size)) * pixel_size - 0.5 * screenSize) / length(screenSize) - offset;
    float uv_len = length(uv);

    float speed = (spin_rotation_speed * SPIN_EASE * 0.2);
    if (is_rotating) {
        speed = TIME * speed;
    }
    speed += 302.2;

    float new_pixel_angle = atan(uv.y, uv.x) + speed - SPIN_EASE * 20.0 * (spin_amount * uv_len + (1.0 - spin_amount));
    vec2 mid = (screenSize / length(screenSize)) * 0.5;
    uv = vec2(uv_len * cos(new_pixel_angle), uv_len * sin(new_pixel_angle)) + mid - mid;

    uv *= 30.0;
    speed = TIME * move_speed;
    vec2 uv2 = uv.x + uv.y;

    for (int i = 0; i < 5; i++) {
        uv2 += sin(max(uv.x, uv.y)) + uv;
        uv += 0.5 * vec2(
        cos(5.1123314 + 0.353 * uv2.y + speed * 0.131121),
        sin(uv2.x - 0.113 * speed)
        );
        uv -= cos(uv.x + uv.y) - sin(uv.x * 0.711 - uv.y);
    }

    float contrast_mod = (0.25 * contrast + 0.5 * spin_amount + 1.2);
    float paint_res = min(2.0, max(0.0, length(uv) * 0.035 * contrast_mod));
    float c1p = max(0.0, 1.0 - contrast_mod * abs(1.0 - paint_res));
    float c2p = max(0.0, 1.0 - contrast_mod * abs(paint_res));
    float c3p = 1.0 - min(1.0, c1p + c2p);

    float ligth = (lighting - 0.2) * max(c1p * 5.0 - 4.0, 0.0) + lighting * max(c2p * 5.0 - 4.0, 0.0);
    vec4 ret_col = (0.3 / contrast) * colour_1 + (1.0 - 0.3 / contrast) * (colour_1 * c1p + colour_2 * c2p + vec4(c3p * colour_3.rgb, c3p * colour_1.a)) + ligth;
    return ret_col;
}

void main() {
    // Screen coordinates: scale TexCoord (0-1) to pixel coordinates
    vec2 screen_coords = TexCoord * iResolution;

    // Generate Balatro effect
    vec4 balatroEffect = effect(iResolution, screen_coords);

    // Option 1: Replace texture with effect
    // FragColor = balatroEffect;

    // Option 2: Blend effect over your texture (recommended for "background with effect")
    vec4 baseColor = texture(ourTexture, TexCoord);
    FragColor = mix(baseColor, balatroEffect, 0.7); // Adjust blend factor as needed
}
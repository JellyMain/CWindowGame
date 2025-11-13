#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);
    float average = (color.r + color.g + color.b) / 3.0;
    FragColor = vec4(average, average, average, color.a);

//     FragColor = texture(screenTexture, TexCoords);
}
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float time;

void main()
{
    vec2 distortedCoord = TexCoord;
    distortedCoord.x += sin(TexCoord.y * 10.0 + time) * 0.05;
    distortedCoord.y += cos(TexCoord.x * 10.0 + time) * 0.05;

    vec4 texColor = texture(ourTexture, distortedCoord);
    FragColor = texColor;
}
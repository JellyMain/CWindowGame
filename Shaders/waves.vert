#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform float time;

void main()
{
    vec2 pos = aPos;

    // Much smaller displacement - 2-5 pixels instead of 20
    pos.x += sin(aPos.y * 0.01 + time * 2.0) * 3.0;
    pos.y += cos(aPos.x * 0.01 + time * 2.0) * 3.0;

    gl_Position = projection * vec4(pos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
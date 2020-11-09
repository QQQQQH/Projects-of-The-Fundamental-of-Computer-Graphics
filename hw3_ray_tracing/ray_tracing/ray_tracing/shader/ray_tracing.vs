#version 330 core

uniform vec2 screenPos;

void main()
{
    gl_Position = vec4(screenPos, 0.0, 1.0);
}

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform vec3 move;

void main()
{
    gl_Position = vec4(aPos.x + move.x, aPos.y + move.y, aPos.z + move.z, 1.0);
    ourColor = aColor;
}
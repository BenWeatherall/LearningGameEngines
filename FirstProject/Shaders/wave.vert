#version 330 core
layout(location = 0) in vec3 position;

uniform float time;
out vec4 vertexColor;
out vec4 loc;

void main()
{
	gl_Position = vec4(position.x + sin(position.y + time), position.y, position.z, 1);
	vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
	loc = vec4(position, 1.0);
};
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 component;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec4 loc;

void main()
{
	gl_Position = projection * view * model * component * vec4(position, 1);
	vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
	loc = vec4(position, 1.0);
};
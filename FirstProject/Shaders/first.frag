#version 330 core
in vec4 loc;
out vec4 color;

uniform vec4 ourColor;

void main()
{
	// color = ourColor;
	color = loc;
};
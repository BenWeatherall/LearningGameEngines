#version 330 core
in vec4 loc;
out vec4 color;

uniform float time;
uniform vec4 ourColor;

void main()
{
	// color = ourColor;
	// color = loc;
	color = vec4(loc.x+ sin(loc.y + time), loc.y+ cos(loc.y + time), loc.z+ tan(loc.y + time), 1);
};
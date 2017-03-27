#version 330 core
in vec4 loc;
in vec4 ourColor;

out vec4 color;

uniform float time;

// Texture samplers (We will revisit to increase texture ranges)
uniform sampler2D texture_01;
uniform sampler2D texture_02;

void main()
{
	color = mix(texture2D(texture_01, loc), texture2D(texture_02, loc)); // We will ignore outColor for now
};
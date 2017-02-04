#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec2 PositionCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float iGlobalTime;

void main()
{
	vec2 vc;
	vc.x = sin(iGlobalTime * length(PositionCoord)) - cos(iGlobalTime);
	vc.y = sin(iGlobalTime * length(PositionCoord));
	color = vec4(vc, 0.5, 1.0f);
	
    // color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}
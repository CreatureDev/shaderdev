#version 130

uniform vec3 ldir;

in vec3 N;
in vec2 tcrd;

uniform sampler2D img;

out vec4 color;

void main()
{
	float str = max(0, dot(N, ldir));
	color =	vec4(str, str, str, 1); 
}


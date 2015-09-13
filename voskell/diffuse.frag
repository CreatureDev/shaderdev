#version 130

uniform vec3 clr0;
uniform vec3 clr1;
uniform float tome;
uniform float cr;
in vec3 N;
in float dist;
uniform vec3 ldir;
out vec4 color;

void main()
{
	vec3 clr = abs(sin(dist + cr * tome)) * clr0 + abs(cos(dist + cr * tome)) * clr1;
	clr = normalize(clr);
	float str = max(0, dot(ldir, N));
	color = vec4(clr * str, 1);
}


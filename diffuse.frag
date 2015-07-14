#version 130

uniform vec3 clr;
in vec3 N;
uniform vec3 ldir;
out vec4 color;

void main()
{
	float str = max(0, dot(ldir, N));
	color = vec4(clr * str, 1);
}


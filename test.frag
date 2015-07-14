#version 130

uniform vec3 clr0;
out vec4 color;

void main()
{	
	color = vec4(clr0, 1);
}


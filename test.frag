#version 130

uniform float time;
uniform float rate;
uniform vec3 clr0;
uniform vec3 clr1;

out vec4 color;

void getdist(out float ret)
{
	ret = gl_FragCoord.x * gl_FragCoord.x;
	ret += gl_FragCoord.y * gl_FragCoord.y;
	ret = sqrt(ret);
}

void main()
{
	float dist, v1, v2;
	vec3 pix;
	getdist(dist);	
	color = vec4(clr0, 1);
}


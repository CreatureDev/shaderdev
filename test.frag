#version 440

uniform float time;
uniform float tome;
uniform float rate;
uniform vec3 clr0;
uniform vec3 clr1

out vec4 color;

void getdist(out ret)
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
	v1 = sin(dist + rate * time);
	v2 = sin(dist - rate * tome);
	pix = clr0 * v1 + clr1 * v2;
	color = vec4(pix, 1);
}


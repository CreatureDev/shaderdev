#version 130

attribute vec3 vp;
attribute vec3 nm;
attribute vec2 tc;

uniform mat4 proj;
uniform mat4 wp;
uniform vec3 rax;
uniform float time;
uniform float rate;

out vec2 tcrd;
out vec3 N;

vec3 quatrot(in vec3 p, in vec4 q)
{
	return p + 2 * cross(cross(p, q.xyz) + q.w * p, q.xyz);
}

void main()
{
	vec4 quat;
	vec3 pos;
	quat.xyz = vec3(sin(time * rate) * normalize(rax));
	quat.w = cos(time * rate);
	pos = quatrot(vp, quat);
	N = quatrot(nm, quat);
	tcrd = tc;
	gl_Position = proj * wp * vec4(pos, 1);
}

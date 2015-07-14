#version 130

attribute vec3 vp;
attribute vec3 nm;
uniform vec3 rax;
uniform mat4 proj;
uniform mat4 wp;
uniform float time;
uniform float rate;

out vec3 N;
out float dist;

vec3 quatrot(in vec3 p, in vec4 q)
{
	return p + 2 * cross(cross(p, q.xyz) + q.w * p, q.xyz);
}

void main()
{
	vec4 quat;
	vec3 pos;
	pos = vp;
	quat.xyz = sin(time * rate) * normalize(rax);
	quat.w = cos(time * rate);
	pos = quatrot(pos, quat);
	N = quatrot(nm, quat);
	dist = dot(vp.xy, vp.xy);
	gl_Position = proj * wp * vec4(pos, 1);
} 

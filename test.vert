#version 130

attribute vec3 vp;
uniform mat4 proj;
uniform mat4 wp;
uniform float time;
uniform float rate;

vec3 quatrot(in vec3 p, in vec4 q)
{
	return p + 2 * cross(cross(p, q.xyz) + q.w * p, q.xyz);
}

void main()
{
	vec4 quat;
	vec3 pos;
	pos = vp;
	pos.y = pos.y + 0.5;
	quat.xyz = vec3(sin(time * rate), 0, 0);
	quat.w = cos(time * rate);
	pos = quatrot(pos, quat);
	gl_Position = proj * wp * vec4(pos, 1);
} 

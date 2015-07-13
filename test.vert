#version 130

attribute vec3 vp;

void main()
{
	gl_Position = vec4(vp, 1);
} 

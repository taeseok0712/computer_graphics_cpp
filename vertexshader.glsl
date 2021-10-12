#version 330 core
layout (location = 0) in vec3 vPos; 
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 modeltransform;


void main()
{

	gl_Position = modeltransform *vec4 (vPos, 1.0);
	outColor = vColor;

}

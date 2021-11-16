#version 330 core
layout (location = 0) in vec3 vPos; 
out vec3 outColor;
uniform mat4 modeltransform;
uniform mat4 viewtransform;
uniform mat4 projectiontransform;
uniform vec3 vColor;

void main()
{

	gl_Position = projectiontransform * viewtransform * modeltransform * vec4 (vPos, 1.0);
	outColor = vColor;

}

#version 330 core
layout (location = 0) in vec3 vPos; 
layout (location = 1) in vec3 vColor;

uniform mat4 modeltransform;
uniform mat4 viewtransform;
uniform mat4 projectiontransform;

out vec3 outColor;

void main()
{

	gl_Position = projectiontransform * viewtransform * modeltransform * vec4 (vPos, 1.0);
	outColor = vColor;

}

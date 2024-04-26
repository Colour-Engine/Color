#type vertex
#version 450 core

layout (location = 0) in vec3 a_Location;

void main()
{
	gl_Position = vec4(a_Location, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}

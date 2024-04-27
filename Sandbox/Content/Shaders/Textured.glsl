#type vertex
#version 450 core

layout (location = 0) in vec3 a_Location;
layout (location = 1) in vec2 a_TexCoord;

struct FVertexOutput
{
	vec2 TexCoord;
};
layout (location = 0) out FVertexOutput Output;

void main()
{
	Output.TexCoord = a_TexCoord;
	gl_Position = vec4(a_Location, 1.0);
}

#type fragment
#version 450 core

struct FVertexOutput
{
	vec2 TexCoord;
};

layout (location = 0) in FVertexOutput Input;
layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, Input.TexCoord);
}
